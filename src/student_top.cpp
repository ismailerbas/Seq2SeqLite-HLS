#include "student_top.h"
#include "student_weight_convert.h"
#include "student_weights_int8.h"

// ============================================================================
// One GRU cell step (reset_after=False, confirmed from qkeras/qrecurrent.py).
//   z  = hard_sigmoid( x.Wz + h_prev.Uz + bz )
//   r  = hard_sigmoid( x.Wr + h_prev.Ur + br )
//   hh = clip( x.Wh + (r*h_prev).Uh + bh , -1, 1 )   [quantized_tanh default
//                                                      == linear clip, NOT
//                                                      real tanh, confirmed
//                                                      empirically]
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

        // hard_sigmoid(v) = clip(0.2*v + 0.5, 0, 1)  -- exact Keras backend formula
        gate_t pre_z = gate_t(sum_z) * gate_t(0.2) + gate_t(0.5);
        if (pre_z < gate_t(0.0)) pre_z = gate_t(0.0);
        else if (pre_z > gate_t(1.0)) pre_z = gate_t(1.0);
        z[u] = pre_z;

        gate_t pre_r = gate_t(sum_r) * gate_t(0.2) + gate_t(0.5);
        if (pre_r < gate_t(0.0)) pre_r = gate_t(0.0);
        else if (pre_r > gate_t(1.0)) pre_r = gate_t(1.0);
        r[u] = pre_r;
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

        // quantized_tanh default (confirmed empirically): clip(v,-1,1) then
        // quantize to 8-bit step 1/128. state_t is ap_fixed<8,1,AP_RND,AP_SAT>
        // so this single assignment performs exactly that clip + round.
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
