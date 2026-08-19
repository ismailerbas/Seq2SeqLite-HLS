#include "student_top_interleaved.h"
#include "student_weight_convert.h"
#include "student_weights_int8.h"

// ============================================================================
// Pixel-interleaved Seq2SeqLite engine.
//
// MATH: byte-for-byte the cell semantics verified in student_top.cpp against
// the real QKeras model (csim 5/5 PASS, max abs err 0.0194 < 0.02):
//   qh   = Q_state(h)          quantized_bits(8,0,symmetric=1,alpha=1.0):
//                              AP_RND_CONV cast to state_t + symmetric clamp
//   z,r  = hard_sigmoid(...)   clip(0.5*x + 0.5, 0, 1), slope 0.5, wide out
//   hh   = quantized_tanh(...) state_t cast + symmetric clamp
//   h'   = z*qh + (1-z)*hh     WIDE blend carried in hstate_t
// Only the SCHEDULE differs: the pipeline iterates over contexts, so each
// context's recurrent dependency (h at t -> h at t+1) is separated by
// NPIX_ILV * ILV_II cycles and never stalls the datapath.
//
// SCHEDULE: for each timestep, a single PIPELINE II=ILV_II loop over the
// NPIX_ILV contexts performs one full GRU step (and, in the decoder phase,
// the dense head) for one context per iteration. All unit/weight loops
// inside the pipelined region are automatically unrolled by HLS; the cyclic
// factor=8 partitioning of the recurrent weights bounds parallel weight
// reads to 8 per gate per unit per cycle, which is what makes II=ILV_II
// the natural schedule instead of a fully combinational 3072-MAC tree.
// The inter-iteration dependence on h_state has distance NPIX_ILV, declared
// to the scheduler explicitly below.
// ============================================================================

// Exact QKeras symmetric 8-bit clamp (duplicated from student_top.cpp, where
// it is file-static): quantized_bits(8,0,symmetric=1) and
// quantized_tanh(8,symmetric=True) clip codes to [-127, +127], so the -128
// code (-1.0) reachable via ap_fixed saturation is raised to -127/128.
static inline state_t ilv_clamp_symmetric(state_t q) {
    if (q == state_t(-1.0)) {
        return state_t(-0.9921875);
    }
    return q;
}

static inline state_t ilv_quantize_state(hstate_t h) {
    return ilv_clamp_symmetric(state_t(h));
}

// One full GRU step for a single context. INLINE so the caller's context
// pipeline absorbs it; all loops below unroll under that pipeline.
static void ilv_gru_step(
    input_t x,
    hstate_t h_ctx[GRU_UNITS],
    const int8_t kernel_z[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_r[INPUT_DIM][GRU_UNITS],
    const int8_t kernel_h[INPUT_DIM][GRU_UNITS],
    const int8_t recurrent_z[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_r[GRU_UNITS][GRU_UNITS],
    const int8_t recurrent_h[GRU_UNITS][GRU_UNITS],
    const int8_t bias_z[GRU_UNITS],
    const int8_t bias_r[GRU_UNITS],
    const int8_t bias_h[GRU_UNITS]
) {
    #pragma HLS INLINE

    state_t qh[GRU_UNITS];
    #pragma HLS ARRAY_PARTITION variable=qh complete dim=1
    for (int k = 0; k < GRU_UNITS; k++) {
        #pragma HLS UNROLL
        qh[k] = ilv_quantize_state(h_ctx[k]);
    }

    gate_t z[GRU_UNITS];
    gate_t r[GRU_UNITS];
    #pragma HLS ARRAY_PARTITION variable=z complete dim=1
    #pragma HLS ARRAY_PARTITION variable=r complete dim=1

    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        acc_t sum_z = 0;
        acc_t sum_r = 0;
        for (int i = 0; i < INPUT_DIM; i++) {
            #pragma HLS UNROLL
            weight_t wz = int8_to_weight(kernel_z[i][u]);
            weight_t wr = int8_to_weight(kernel_r[i][u]);
            #pragma HLS BIND_OP variable=sum_z op=mul impl=dsp latency=2
            #pragma HLS BIND_OP variable=sum_r op=mul impl=dsp latency=2
            sum_z += acc_t(x) * acc_t(wz);
            sum_r += acc_t(x) * acc_t(wr);
        }
        for (int k = 0; k < GRU_UNITS; k++) {
            #pragma HLS UNROLL
            weight_t uz = int8_to_weight(recurrent_z[k][u]);
            weight_t ur = int8_to_weight(recurrent_r[k][u]);
            sum_z += acc_t(qh[k]) * acc_t(uz);
            sum_r += acc_t(qh[k]) * acc_t(ur);
        }
        bias_t bz = int8_to_bias(bias_z[u]);
        bias_t br = int8_to_bias(bias_r[u]);
        sum_z += acc_t(bz);
        sum_r += acc_t(br);

        acc_t pre_z_acc = sum_z * acc_t(0.5) + acc_t(0.5);
        if (pre_z_acc < acc_t(0.0)) pre_z_acc = acc_t(0.0);
        else if (pre_z_acc > acc_t(1.0)) pre_z_acc = acc_t(1.0);
        z[u] = gate_t(pre_z_acc);

        acc_t pre_r_acc = sum_r * acc_t(0.5) + acc_t(0.5);
        if (pre_r_acc < acc_t(0.0)) pre_r_acc = acc_t(0.0);
        else if (pre_r_acc > acc_t(1.0)) pre_r_acc = acc_t(1.0);
        r[u] = gate_t(pre_r_acc);
    }

    hstate_t r_h_prev[GRU_UNITS];
    #pragma HLS ARRAY_PARTITION variable=r_h_prev complete dim=1
    for (int k = 0; k < GRU_UNITS; k++) {
        #pragma HLS UNROLL
        r_h_prev[k] = hstate_t(r[k] * qh[k]);
    }

    for (int u = 0; u < GRU_UNITS; u++) {
        #pragma HLS UNROLL
        acc_t sum_h = 0;
        for (int i = 0; i < INPUT_DIM; i++) {
            #pragma HLS UNROLL
            weight_t wh = int8_to_weight(kernel_h[i][u]);
            #pragma HLS BIND_OP variable=sum_h op=mul impl=dsp latency=2
            sum_h += acc_t(x) * acc_t(wh);
        }
        for (int k = 0; k < GRU_UNITS; k++) {
            #pragma HLS UNROLL
            weight_t uh = int8_to_weight(recurrent_h[k][u]);
            sum_h += acc_t(r_h_prev[k]) * acc_t(uh);
        }
        bias_t bh = int8_to_bias(bias_h[u]);
        sum_h += acc_t(bh);

        state_t hh = ilv_clamp_symmetric(state_t(sum_h));

        gate_t one_minus_z = gate_t(1.0) - z[u];
        h_ctx[u] = hstate_t(z[u] * qh[u] + one_minus_z * hstate_t(hh));
    }
}

void student_infer_batch_interleaved(
    input_t  tpsf_in[NPIX_ILV][SEQ_LEN],
    output_t sfd_out[NPIX_ILV][SEQ_LEN][N_OUT]
) {
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_z complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_r complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_kernel_h complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_z cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_z complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_r cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_r complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_h cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sencgru_recurrent_kernel_h complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_z cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_z complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_r cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_r complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_h cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdecgru_recurrent_kernel_h complete dim=2
    #pragma HLS ARRAY_PARTITION variable=sdec_dense_kernel cyclic factor=8 dim=1
    #pragma HLS ARRAY_PARTITION variable=sdec_dense_kernel complete dim=2

    // Replicated per-context recurrent state: the Bpixel term, physically.
    // dim=2 complete so one context's full 32-unit state vector is read and
    // written per pipeline iteration; dim=1 (contexts) maps to memory.
    static hstate_t h_state[NPIX_ILV][GRU_UNITS];
    #pragma HLS ARRAY_PARTITION variable=h_state complete dim=2

    // ---- init all contexts: h = 0 ----
    INIT_CTX: for (int p = 0; p < NPIX_ILV; p++) {
        #pragma HLS PIPELINE II=1
        for (int u = 0; u < GRU_UNITS; u++) {
            #pragma HLS UNROLL
            h_state[p][u] = hstate_t(0.0);
        }
    }

    // ---- encoder: timestep-outer, context-inner. Each context's
    //      write->read of h_state is NPIX_ILV iterations apart. ----
    ENC_T: for (int t = 0; t < SEQ_LEN; t++) {
        ENC_CTX: for (int p = 0; p < NPIX_ILV; p++) {
            #pragma HLS PIPELINE II=ILV_II
            #pragma HLS dependence variable=h_state inter distance=64 true
            hstate_t h_ctx[GRU_UNITS];
            #pragma HLS ARRAY_PARTITION variable=h_ctx complete dim=1
            for (int u = 0; u < GRU_UNITS; u++) {
                #pragma HLS UNROLL
                h_ctx[u] = h_state[p][u];
            }
            ilv_gru_step(
                tpsf_in[p][t], h_ctx,
                sencgru_kernel_z, sencgru_kernel_r, sencgru_kernel_h,
                sencgru_recurrent_kernel_z, sencgru_recurrent_kernel_r, sencgru_recurrent_kernel_h,
                sencgru_bias_z, sencgru_bias_r, sencgru_bias_h
            );
            for (int u = 0; u < GRU_UNITS; u++) {
                #pragma HLS UNROLL
                h_state[p][u] = h_ctx[u];
            }
        }
    }

    // ---- decoder: state carries over from the encoder final state (the
    //      wide float blend, matching initial_state=encoder_state); input
    //      is constant zero; dense head evaluated per context per step. ----
    DEC_T: for (int t = 0; t < SEQ_LEN; t++) {
        DEC_CTX: for (int p = 0; p < NPIX_ILV; p++) {
            #pragma HLS PIPELINE II=ILV_II
            #pragma HLS dependence variable=h_state inter distance=64 true
            hstate_t h_ctx[GRU_UNITS];
            #pragma HLS ARRAY_PARTITION variable=h_ctx complete dim=1
            for (int u = 0; u < GRU_UNITS; u++) {
                #pragma HLS UNROLL
                h_ctx[u] = h_state[p][u];
            }
            ilv_gru_step(
                input_t(0.0), h_ctx,
                sdecgru_kernel_z, sdecgru_kernel_r, sdecgru_kernel_h,
                sdecgru_recurrent_kernel_z, sdecgru_recurrent_kernel_r, sdecgru_recurrent_kernel_h,
                sdecgru_bias_z, sdecgru_bias_r, sdecgru_bias_h
            );
            for (int u = 0; u < GRU_UNITS; u++) {
                #pragma HLS UNROLL
                h_state[p][u] = h_ctx[u];
            }
            for (int o = 0; o < N_OUT; o++) {
                #pragma HLS UNROLL
                dense_acc_t sum_o = 0;
                for (int u = 0; u < GRU_UNITS; u++) {
                    #pragma HLS UNROLL
                    weight_t wo = int8_to_weight(sdec_dense_kernel[u][o]);
                    sum_o += dense_acc_t(h_ctx[u]) * dense_acc_t(wo);
                }
                bias_t bo = int8_to_bias(sdec_dense_bias[o]);
                sum_o += dense_acc_t(bo);
                sfd_out[p][t][o] = output_t(sum_o);
            }
        }
    }
}

void student_infer_frame_interleaved(
    const input_t *tpsf_frame_in,
    output_t      *sfd_frame_out
) {
    #pragma HLS INTERFACE m_axi port=tpsf_frame_in offset=slave bundle=gmem_ilv_in depth=33750000
    #pragma HLS INTERFACE m_axi port=sfd_frame_out offset=slave bundle=gmem_ilv_out depth=101250000
    #pragma HLS INTERFACE s_axilite port=return

    input_t  tpsf_batch[NPIX_ILV][SEQ_LEN];
    output_t sfd_batch[NPIX_ILV][SEQ_LEN][N_OUT];

    const int num_batches = (ILV_TOTAL_PIXELS + NPIX_ILV - 1) / NPIX_ILV;

    BATCH: for (int b = 0; b < num_batches; b++) {
        int base = b * NPIX_ILV;

        // Load: guarded so the final partial batch (250,000 = 64*3906 + 16)
        // pads unused contexts with zeros; their outputs are never stored.
        LOAD: for (int p = 0; p < NPIX_ILV; p++) {
            int pix = base + p;
            LOAD_T: for (int t = 0; t < SEQ_LEN; t++) {
                #pragma HLS PIPELINE II=1
                if (pix < ILV_TOTAL_PIXELS) {
                    tpsf_batch[p][t] = tpsf_frame_in[pix * SEQ_LEN + t];
                } else {
                    tpsf_batch[p][t] = input_t(0.0);
                }
            }
        }

        student_infer_batch_interleaved(tpsf_batch, sfd_batch);

        STORE: for (int p = 0; p < NPIX_ILV; p++) {
            int pix = base + p;
            if (pix < ILV_TOTAL_PIXELS) {
                STORE_T: for (int t = 0; t < SEQ_LEN; t++) {
                    STORE_O: for (int o = 0; o < N_OUT; o++) {
                        #pragma HLS PIPELINE II=1
                        sfd_frame_out[(pix * SEQ_LEN + t) * N_OUT + o] = sfd_batch[p][t][o];
                    }
                }
            }
        }
    }
}