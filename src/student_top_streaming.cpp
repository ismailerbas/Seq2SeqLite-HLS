#include "student_top_streaming.h"

// ============================================================================
// One lane: pulls PIXELS_PER_LANE pixels sequentially from its own m_axi
// buffer, runs each through the existing, already-synthesized-and-verified
// student_infer_pixel (150 DSP, 46154 LUT, 111653-cycle latency per pixel,
// measured from the real solution1 csynth report), and writes the result
// back to its own output buffer. This is NOT inlined and NOT unrolled --
// it is a plain sequential loop, so this lane synthesizes as exactly ONE
// physical instance of student_infer_pixel, reused PIXELS_PER_LANE times,
// identical in resource cost to the single-pixel design already verified.
// ============================================================================
static void lane_worker(
    input_t  *tpsf_in_lane,
    output_t *sfd_out_lane
) {
    for (int local_idx = 0; local_idx < PIXELS_PER_LANE; local_idx++) {

        input_t  tpsf_in[SEQ_LEN];
        output_t sfd_out[SEQ_LEN][N_OUT];

        int base_in = local_idx * SEQ_LEN;
        READ_PIXEL: for (int t = 0; t < SEQ_LEN; t++) {
            #pragma HLS PIPELINE II=1
            tpsf_in[t] = tpsf_in_lane[base_in + t];
        }

        student_infer_pixel(tpsf_in, sfd_out);

        int base_out = local_idx * SEQ_LEN * N_OUT;
        WRITE_PIXEL_T: for (int t = 0; t < SEQ_LEN; t++) {
            WRITE_PIXEL_O: for (int o = 0; o < N_OUT; o++) {
                #pragma HLS PIPELINE II=1
                sfd_out_lane[base_out + t * N_OUT + o] = sfd_out[t][o];
            }
        }
    }
}

// ============================================================================
// Top-level 4-lane streaming frame inference. Each lane gets its own
// dedicated m_axi bundle (gmem_in0..3, gmem_out0..3) so the 4 physically
// separate AXI master ports let the DATAFLOW scheduler run all 4 lanes
// truly concurrently, with no risk of the tool inserting a false
// dependency between lanes the way a single shared pointer argument would.
//
// depth= values below are the exact real element counts for this frame
// size, not placeholders: PIXELS_PER_LANE (62500) * SEQ_LEN (135) = 8437500
// for each input lane buffer, and PIXELS_PER_LANE * SEQ_LEN * N_OUT (3)
// = 25312500 for each output lane buffer.
// ============================================================================
void student_infer_frame_streaming(
    input_t  *tpsf_in_lane0,
    input_t  *tpsf_in_lane1,
    input_t  *tpsf_in_lane2,
    input_t  *tpsf_in_lane3,
    output_t *sfd_out_lane0,
    output_t *sfd_out_lane1,
    output_t *sfd_out_lane2,
    output_t *sfd_out_lane3
) {
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane0  offset=slave bundle=gmem_in0  depth=8437500
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane1  offset=slave bundle=gmem_in1  depth=8437500
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane2  offset=slave bundle=gmem_in2  depth=8437500
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane3  offset=slave bundle=gmem_in3  depth=8437500
    #pragma HLS INTERFACE m_axi port=sfd_out_lane0  offset=slave bundle=gmem_out0 depth=25312500
    #pragma HLS INTERFACE m_axi port=sfd_out_lane1  offset=slave bundle=gmem_out1 depth=25312500
    #pragma HLS INTERFACE m_axi port=sfd_out_lane2  offset=slave bundle=gmem_out2 depth=25312500
    #pragma HLS INTERFACE m_axi port=sfd_out_lane3  offset=slave bundle=gmem_out3 depth=25312500

    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane0  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane1  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane2  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane3  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane0  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane1  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane2  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane3  bundle=control
    #pragma HLS INTERFACE s_axilite port=return         bundle=control

    #pragma HLS DATAFLOW

    lane_worker(tpsf_in_lane0, sfd_out_lane0);
    lane_worker(tpsf_in_lane1, sfd_out_lane1);
    lane_worker(tpsf_in_lane2, sfd_out_lane2);
    lane_worker(tpsf_in_lane3, sfd_out_lane3);
}