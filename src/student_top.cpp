#include "student_top.h"
#include "student_weight_convert.h"
#include "student_weights_int8.h"

// ============================================================================
// One GRU cell step, bit-matching qkeras/qrecurrent.py QGRUCell.call() for
// THIS model's exact configuration, verified from build_student_model() in
// Seq2SeqLite.py (the training repo) and the QKeras 0.9.0 package source:
//
//   activation           = quantized_tanh(bits=8, symmetric=True)
//   recurrent_activation = 'hard_sigmoid' (QGRUCell default -> QKeras's own
//                          hard_sigmoid: clip(0.5*x + 0.5, 0, 1), slope 0.5)
//   kernel/recurrent/bias/state quantizers = quantized_bits(8, 0, 1, alpha=1.0)
//                          (3rd positional arg is SYMMETRIC=1)
//   reset_after = False
//
// QGRUCell.call() with a state_quantizer performs, per step:
//   qh   = Q_state(h)                      quantize carried state ON ENTRY
//   z    = hard_sigmoid( x.Wz + qh.Uz + bz )              float output
//   r    = hard_sigmoid( x.Wr + qh.Ur + br )              float output
//   pre  = x.Wh + (r*qh).Uh + bh           r*qh is a FLOAT product
//   hh   = quantized_tanh(pre)             1/128 grid, clip [-127/128,127/128]
//   h    = z*qh + (1-z)*hh                 FLOAT blend: carried to the next
//                                          step AND emitted as the layer
//                                          output that QDense consumes
//
// Q_state (quantized_bits(8,0,symmetric=1,alpha=1.0), from
// quantized_bits.__call__): xq = clip(round_half_even(x*128), -127, 127)/128.
// Symmetric clip means the -128 code NEVER occurs: floor is -127/128, and
// rounding is tf.round = round half to even. state_t is
// ap_fixed<8,1,AP_RND_CONV,AP_SAT> (AP_RND_CONV == round half to even), whose
// only deviation is that its saturation floor is -1.0 (code -128); the
// clamp_symmetric() helper below bumps that single code to -127/128, making
// the cast + clamp EXACTLY Q_state. quantized_tanh(8, symmetric=True) with
// the default "hard" internal sigmoid reduces to the same operation applied
// to clip(x,-1,1), so hh uses the identical cast + clamp.
//
// PRECISION RULES (each verified against the call() source above):
//   - the CARRIED state h and the blend are WIDE (hstate_t): the model
//     carries the raw float blend and only quantizes on entry.
//   - the r*qh product feeding the Uh matmul is WIDE: never quantized.
//   - z, r are WIDE (gate_t): hard_sigmoid output is raw float.
//   - qh and hh are the ONLY 8-bit values, because Q_state and
//     quantized_tanh are the only quantizers in the cell.
//
// TIMING (target clock period 4.5 ns): gate/candidate loops pipelined at
// II=4 against the "cyclic factor=8" recurrent-weight partitioning at the
// call site (8 of 32 weights readable per cycle -> 32/8 = 4 cycles), with
// BIND_OP pinning the accumulation multiplies onto DSP48 slices (2-cycle
// latency). The z/r/h matmul operands are back to 8-bit qh x 8-bit weight,
// and the widest MAC operand anywhere (24-bit r_h_prev x 8-bit weight)
// fits the DSP48E1 25x18 multiplier, so the II=4 architecture is unchanged.
// ============================================================================

// Exact QKeras symmetric 8-bit clamp: quantized_bits(8,0,symmetric=1) and
// quantized_tanh(8,symmetric=True) both clip codes to [-127, +127], so the
// -128 code (-1.0) that ap_fixed saturation can produce must be raised to
// -127/128 = -0.9921875 (exactly representable in state_t).
static inline state_t clamp_symmetric(state_t q) {
    if (q == state_t(-1.0)) {
        return state_t(-0.9921875);
    }
    return q;
}

// Q_state: quantized_bits(8, 0, symmetric=1, alpha=1.0) applied to the wide
// carried state on entry to the cell. state_t's AP_RND_CONV cast performs
// the round-half-even onto the 1/128 grid; clamp_symmetric applies the
// symmetric [-127/128, +127/128] clip.
static inline state_t quantize_state(hstate_t h) {
    return clamp_symmetric(state_t(h));
}

static void gru_cell_step(
    input_t x,
    hstate_t h_prev[GRU_UNITS],
    const int8_t kernel_z[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_r[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_h[INPUT_DIM][GRU_UNITS],
    const int8_t recurrent_z[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_r[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_h[GRU_UNITS][GRU_UNITS],
    const int8_t bias_z[GRU_UNITS],
    const int8_t bias_r[GRU_UNITS],
    const int8_t bias_h[GRU_UNITS],
    hstate_t h_new[GRU_UNITS]
) {
    // ---- Q_state on entry: the quantized state qh is what EVERY use of the
    //      previous state sees inside the cell (all three matmuls, the reset
    //      product, and the blend), exactly as in QGRUCell.call(). ----
    state_t qh[GRU_UNITS];
    for (int k = 0; k < GRU_UNITS; k++) {
        #pragma HLS UNROLL
        qh[k] = quantize_state(h_prev[k]);
    }

    gate_t z[GRU_UNITS];
    gate_t r[GRU_UNITS];

    // ---- update gate z and reset gate r ----
    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS PIPELINE II=4

        acc_t sum_z = 0;
        acc_t sum_r = 0;

        for (int i = 0; i < INPUT_DIM; i++) {
            weight_t wz = int8_to_weight(kernel_z[i][u]);
            weight_t wr = int8_to_weight(kernel_r[i][u]);
            #pragma HLS BIND_OP variable=sum_z op=mul impl=dsp latency=2
            #pragma HLS BIND_OP variable=sum_r op=mul impl=dsp latency=2
            sum_z += acc_t(x) * acc_t(wz);
            sum_r += acc_t(x) * acc_t(wr);
        }
        for (int k = 0; k < GRU_UNITS; k++) {
            weight_t uz = int8_to_weight(recurrent_z[k][u]);
            weight_t ur = int8_to_weight(recurrent_r[k][u]);
            sum_z += acc_t(qh[k]) * acc_t(uz);
            sum_r += acc_t(qh[k]) * acc_t(ur);
        }

        bias_t bz = int8_to_bias(bias_z[u]);
        bias_t br = int8_to_bias(bias_r[u]);
        sum_z += acc_t(bz);
        sum_r += acc_t(br);

        // QGRUCell's default 'hard_sigmoid' STRING resolves through
        // get_quantizer's safe_eval against qkeras.quantizers' own
        // namespace: clip(0.5*x + 0.5, 0, 1), slope 0.5, output raw
        // (unquantized). Scaling and clipping performed entirely in acc_t,
        // then cast down to gate_t only after the value is already in [0,1].
        acc_t pre_z_acc = sum_z * acc_t(0.5) + acc_t(0.5);
        if (pre_z_acc < acc_t(0.0)) pre_z_acc = acc_t(0.0);
        else if (pre_z_acc > acc_t(1.0)) pre_z_acc = acc_t(1.0);
        z[u] = gate_t(pre_z_acc);

        acc_t pre_r_acc = sum_r * acc_t(0.5) + acc_t(0.5);
        if (pre_r_acc < acc_t(0.0)) pre_r_acc = acc_t(0.0);
        else if (pre_r_acc > acc_t(1.0)) pre_r_acc = acc_t(1.0);
        r[u] = gate_t(pre_r_acc);
    }

    // ---- candidate hidden state hh (reset applied BEFORE recurrent matmul,
    //      since reset_after=False). The reset product is r * qh -- the
    //      QUANTIZED state -- but the PRODUCT itself stays WIDE, matching
    //      QGRUCell.call's raw-float r * h_tm1 product. ----
    hstate_t r_h_prev[GRU_UNITS];
    for (int k = 0; k < GRU_UNITS; k++) {
        #pragma HLS UNROLL
        r_h_prev[k] = hstate_t(r[k] * qh[k]);
    }

    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS PIPELINE II=4

        acc_t sum_h = 0;

        for (int i = 0; i < INPUT_DIM; i++) {
            weight_t wh = int8_to_weight(kernel_h[i][u]);
            #pragma HLS BIND_OP variable=sum_h op=mul impl=dsp latency=2
            sum_h += acc_t(x) * acc_t(wh);
        }
        for (int k = 0; k < GRU_UNITS; k++) {
            weight_t uh = int8_to_weight(recurrent_h[k][u]);
            sum_h += acc_t(r_h_prev[k]) * acc_t(uh);
        }

        bias_t bh = int8_to_bias(bias_h[u]);
        sum_h += acc_t(bh);

        // quantized_tanh(8, symmetric=True), default "hard" internal
        // sigmoid: clip(x,-1,1) rounded half-to-even onto the 1/128 grid,
        // clipped to [-127/128, +127/128]. state_t cast (AP_RND_CONV,
        // AP_SAT) + clamp_symmetric is exactly that.
        state_t hh = clamp_symmetric(state_t(sum_h));

        // h_new = z*qh + (1-z)*hh -- the blend uses the QUANTIZED state qh
        // (verified: QGRUCell.call computes h = z*h_tm1 + (1-z)*hh where
        // h_tm1 is the state-quantized value), and the RESULT is carried
        // WIDE: the model's carried/output state is this raw float blend,
        // re-quantized only on entry to the next step.
        gate_t one_minus_z = gate_t(1.0) - z[u];
        hstate_t h_upd = hstate_t(z[u] * qh[u] +
                                  one_minus_z * hstate_t(hh));
        h_new[u] = h_upd;
    }
}

// ============================================================================
// Top-level single-pixel inference.
//
// TIMING/MEMORY FIX (target clock period 4.5 ns):
//   1. The outer per-timestep loops (encoder and decoder) no longer carry
//      "PIPELINE II=1 rewind". Nesting an II=1 pipeline around gru_cell_step
//      (whose own loops now run at II=4) is an unsatisfiable constraint that
//      forced the scheduler into the exhaustive search that produced the
//      50 GB Out-of-Memory crash. The outer loops now run as plain sequential
//      loops; each call into gru_cell_step is still internally pipelined.
//   2. ARRAY_PARTITION on every recurrent_kernel_* array is changed from
//      "complete" to "cyclic factor=8" -- this caps parallel weight reads to
//      8 per cycle, which is what allows the II=4 scheduling above to be
//      physically realizable instead of silently falling back to a fully
//      unrolled combinational tree.
//   3. sdec_dense_kernel now has an explicit "cyclic factor=8" partition and
//      the QDense output loop below runs at II=4 for the same reason -- it
//      has the identical 32-wide MAC-tree-in-one-cycle problem the GRU gates
//      had.
// ============================================================================
void student_infer_pixel(
    input_t  tpsf_in[SEQ_LEN],
    output_t sfd_out[SEQ_LEN][N_OUT]
) {
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_z complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_r complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_h complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_z cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_r cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_h cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_z cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_r cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_h cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdec_dense_kernel cyclic factor=8 dim=1

    hstate_t h_enc[GRU_UNITS];
    hstate_t h_dec[GRU_UNITS];

    // ---- encoder: init h1=0 (Algorithm 3, line 3), sweep t=1..T ----
    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        h_enc[u] = hstate_t(0.0);
    }

    for (int t = 0; t < SEQ_LEN; t++) {
        hstate_t h_next[GRU_UNITS];
        gru_cell_step(
            tpsf_in[t], h_enc,
            sencgru_kernel_z, sencgru_kernel_r, sencgru_kernel_h,
            sencgru_recurrent_kernel_z, sencgru_recurrent_kernel_r, sencgru_recurrent_kernel_h,
            sencgru_bias_z, sencgru_bias_r, sencgru_bias_h,
            h_next
        );
        for (int u = 0; u < GRU_UNITS; u++) {
            #pragma HLS UNROLL
            h_enc[u] = h_next[u];
        }
    }

    // ---- decoder: init state = encoder final state (the WIDE float blend,
    //      matching initial_state=encoder_state in build_student_model --
    //      the decoder's own Q_state quantizes it on entry inside
    //      gru_cell_step), input is always zero (confirmed:
    //      dec_arr = np.zeros_like(enc_arr) in make_kd_dataset /
    //      run_inference) ----
    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        h_dec[u] = h_enc[u];
    }

    for (int t = 0; t < SEQ_LEN; t++) {
        input_t zero_input = input_t(0.0);
        hstate_t h_next[GRU_UNITS];
        gru_cell_step(
            zero_input, h_dec,
            sdecgru_kernel_z, sdecgru_kernel_r, sdecgru_kernel_h,
            sdecgru_recurrent_kernel_z, sdecgru_recurrent_kernel_r, sdecgru_recurrent_kernel_h,
            sdecgru_bias_z, sdecgru_bias_r, sdecgru_bias_h,
            h_next
        );
        for (int u = 0; u < GRU_UNITS; u++) {
            #pragma HLS UNROLL
            h_dec[u] = h_next[u];
        }

        // ---- QDense output head: y = h_dec . W + b, LINEAR, no output
        //      quantizer (activation="linear"). VERIFIED from
        //      build_student_model: QDense consumes decoder_outputs, which
        //      are the GRU layer's per-step outputs = the raw WIDE float
        //      blend, NOT the quantized state. h_dec here is exactly that
        //      blend. ----
        for (int o = 0; o < N_OUT; o++) {
            #pragma HLS PIPELINE II=4
            dense_acc_t sum_o = 0;
            for (int u = 0; u < GRU_UNITS; u++) {
                weight_t wo = int8_to_weight(sdec_dense_kernel[u][o]);
                sum_o += dense_acc_t(h_dec[u]) * dense_acc_t(wo);
            }
            bias_t bo = int8_to_bias(sdec_dense_bias[o]);
            sum_o += dense_acc_t(bo);
            sfd_out[t][o] = output_t(sum_o);
        }
    }
}
// ============================================================================
// Post-processing: per-pixel lifetime extraction, replicating
// extract_lifetimes() from eval_experimental.py exactly. Uses double
// arithmetic for the trapezoidal integration and divisions -- this is
// correct and sufficient for C-simulation / verification against real
// ground truth. If this needs to run as synthesized RTL on the FPGA
// (not just in csim), the trapezoidal sum and divisions below need to be
// re-expressed in fixed-point (ap_fixed) with an HLS-synthesizable divide
// core before csynth_design will accept it -- flag that as a separate
// task once this passes csim as written.
// ============================================================================
void extract_lifetimes_pixel(
    output_t sfd_out[SEQ_LEN][N_OUT],
    double &tau1,
    double &tau2,
    double &fret
) {
    double h = GATE_WIDTH_NS;

    double ch1_first = (double)sfd_out[0][1];
    double ch1_last  = (double)sfd_out[SEQ_LEN - 1][1];
    double ch2_first = (double)sfd_out[0][2];
    double ch2_last  = (double)sfd_out[SEQ_LEN - 1][2];

    double sum1 = 0.0;
    double sum2 = 0.0;
    for (int t = 1; t < SEQ_LEN - 1; t++) {
        sum1 += (double)sfd_out[t][1];
        sum2 += (double)sfd_out[t][2];
    }

    // Trapezoidal rule for uniformly spaced samples, matching numpy.trapz
    // exactly: h * (0.5*y[0] + y[1] + ... + y[N-2] + 0.5*y[N-1]).
    double int1 = h * (0.5 * ch1_first + sum1 + 0.5 * ch1_last);
    double int2 = h * (0.5 * ch2_first + sum2 + 0.5 * ch2_last);

    double amp1 = (double)sfd_out[0][1];
    double amp2 = (double)sfd_out[0][2];

    tau1 = (amp1 > 1e-6) ? (int1 / amp1) : 0.0;
    tau2 = (amp2 > 1e-6) ? (int2 / amp2) : 0.0;

    double denom = amp1 + amp2;
    fret = (denom > 1e-6) ? (amp1 / denom) : 0.5;
}