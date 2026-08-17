#include "student_top.h"
#include "student_weight_convert.h"
#include "student_weights_int8.h"

// ============================================================================
// One GRU cell step (reset_after=False, confirmed from qkeras/qrecurrent.py
// QGRUCell.call() source, verified line-by-line against the actual
// implementation).
//   z  = hard_sigmoid( x.Wz + h_prev.Uz + bz )
//   r  = hard_sigmoid( x.Wr + h_prev.Ur + br )
//   hh = quantized_tanh( x.Wh + (r*h_prev).Uh + bh )
//   h_new = z*h_prev + (1-z)*hh
//
// kernel_z/r/h    : [INPUT_DIM][GRU_UNITS]  int8 codes
// recurrent_z/r/h : [GRU_UNITS][GRU_UNITS]  int8 codes
// bias_z/r/h      : [GRU_UNITS]             int8 codes
//
// TIMING FIX (target clock period 4.5 ns): the recurrent accumulation loops
// below are pipelined at II=4 instead of II=1. Each recurrent_* array is
// partitioned "cyclic factor=8" at the call site (see student_infer_pixel),
// so only 8 of the 32 recurrent weights are readable per cycle. That forces
// the k-loop's 32-way multiply-accumulate tree to be scheduled across 4
// cycles (32/8) instead of being flattened into a single combinational
// stage, which is what produced the 6.75 ns 'mul' delay on the 'r' gate.
// BIND_OP pins the accumulation multiplies onto DSP48 slices (2-cycle
// latency) instead of LUT-fabric multipliers, which is required to close
// timing at 4.5 ns.
//
// CORRECTNESS FIX (hard_sigmoid overflow): sum_z/sum_r are the RAW, UNSCALED
// pre-activation accumulator values in acc_t (ap_fixed<32,10,AP_TRN,AP_WRAP>,
// 10 integer bits -- sized to hold a 32-term MAC sum without overflow).
// The scaling (*0.2 + 0.5) and the [0,1] clip are performed entirely in
// acc_t (which has the integer-bit headroom to represent the raw sum
// exactly), and only downcast to gate_t after the value is already clipped
// into [0,1] -- a range gate_t represents exactly, so the final narrowing
// cast is lossless. Casting the raw unscaled sum directly into gate_t
// (ap_fixed<16,2,AP_TRN,AP_WRAP>, only 2 integer bits) before scaling would
// silently wrap instead of saturate.
//
// quantized_tanh VERIFIED (from qkeras.quantizers.quantized_tanh.__call__,
// via inspect.getsource on the actual installed package): with
// use_real_tanh=False (the default, never overridden by qa() in
// extract_student_weights.py), p = 2*_sigmoid(x) - 1, where _sigmoid is
// QKeras's own hard_sigmoid: clip(0.5*x + 0.5, 0, 1). Algebraically, for
// any x in [-1,1], 2*clip(0.5x+0.5,0,1) - 1 = 2*(0.5x+0.5) - 1 = x exactly;
// outside that range it saturates to -1 or 1. This reduces identically to
// clip(x, -1, 1), which is exactly what casting into state_t (ap_fixed<8,1,
// AP_RND,AP_SAT>) already performs via its own saturate-and-round behavior.
// No separate sigmoid/exp computation is needed or correct here.
// ============================================================================
static void gru_cell_step(
    input_t x,
    state_t h_prev[GRU_UNITS],
    const int8_t kernel_z[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_r[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_h[INPUT_DIM][GRU_UNITS],
    const int8_t recurrent_z[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_r[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_h[GRU_UNITS][GRU_UNITS],
    const int8_t bias_z[GRU_UNITS],
    const int8_t bias_r[GRU_UNITS],
    const int8_t bias_h[GRU_UNITS],
    state_t h_new[GRU_UNITS]
) {
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
            sum_z += acc_t(h_prev[k]) * acc_t(uz);
            sum_r += acc_t(h_prev[k]) * acc_t(ur);
        }

        bias_t bz = int8_to_bias(bias_z[u]);
        bias_t br = int8_to_bias(bias_r[u]);
        sum_z += acc_t(bz);
        sum_r += acc_t(br);

        // QGRUCell's 'hard_sigmoid' STRING resolves to QKeras's OWN internal
        // hard_sigmoid, NOT the standard tf.keras.backend one -- confirmed
        // via inspect.getsource(cell.recurrent_activation) on the real
        // instantiated QGRUCell: clip(0.5*x + 0.5, 0, 1), slope 0.5, not 0.2.
        // Scaling and clipping performed entirely in acc_t, then cast down
        // to gate_t only after the value is already in [0,1].
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
    //      since reset_after=False) ----
    state_t r_h_prev[GRU_UNITS];
    for (int k = 0; k < GRU_UNITS; k++) {
        #pragma HLS UNROLL
        r_h_prev[k] = state_t(r[k] * gate_t(h_prev[k]));
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

        // quantized_tanh, VERIFIED: with use_real_tanh=False (default),
        // p = 2*_sigmoid(x)-1 where _sigmoid = clip(0.5x+0.5,0,1), which
        // reduces algebraically to clip(x,-1,1). state_t is ap_fixed<8,1,
        // AP_RND,AP_SAT>, so this cast performs exactly that clip + round.
        state_t hh = state_t(sum_h);

        // h_new = z*h_prev + (1-z)*hh
        gate_t one_minus_z = gate_t(1.0) - z[u];
        state_t h_upd = state_t(gate_t(z[u]) * gate_t(h_prev[u]) +
                                 one_minus_z * gate_t(hh));
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

    state_t h_enc[GRU_UNITS];
    state_t h_dec[GRU_UNITS];

    // ---- encoder: init h1=0 (Algorithm 3, line 3), sweep t=1..T ----
    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        h_enc[u] = state_t(0.0);
    }

    for (int t = 0; t < SEQ_LEN; t++) {
        state_t h_next[GRU_UNITS];
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

    // ---- decoder: init state = encoder final state (Algorithm 3, line 8),
    //      input is always zero (confirmed: dec_arr = np.zeros_like(enc_arr)
    //      in make_kd_dataset / run_inference) ----
    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        h_dec[u] = h_enc[u];
    }

    for (int t = 0; t < SEQ_LEN; t++) {
        input_t zero_input = input_t(0.0);
        state_t h_next[GRU_UNITS];
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
        //      quantizer attached (activation="linear", qd() only quantizes
        //      the kernel/bias at training time). ----
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