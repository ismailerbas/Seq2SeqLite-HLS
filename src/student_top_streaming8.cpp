#include "student_top_streaming8.h"

static void lane_worker8(
    input_t  *tpsf_in_lane,
    output_t *sfd_out_lane
) {
    for (int local_idx = 0; local_idx < PIXELS_PER_LANE8; local_idx++) {

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

void student_infer_frame_streaming8(
    input_t  *tpsf_in_lane0,
    input_t  *tpsf_in_lane1,
    input_t  *tpsf_in_lane2,
    input_t  *tpsf_in_lane3,
    input_t  *tpsf_in_lane4,
    input_t  *tpsf_in_lane5,
    input_t  *tpsf_in_lane6,
    input_t  *tpsf_in_lane7,
    output_t *sfd_out_lane0,
    output_t *sfd_out_lane1,
    output_t *sfd_out_lane2,
    output_t *sfd_out_lane3,
    output_t *sfd_out_lane4,
    output_t *sfd_out_lane5,
    output_t *sfd_out_lane6,
    output_t *sfd_out_lane7
) {
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane0  offset=slave bundle=gmem_in0  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane1  offset=slave bundle=gmem_in1  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane2  offset=slave bundle=gmem_in2  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane3  offset=slave bundle=gmem_in3  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane4  offset=slave bundle=gmem_in4  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane5  offset=slave bundle=gmem_in5  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane6  offset=slave bundle=gmem_in6  depth=4218750
    #pragma HLS INTERFACE m_axi port=tpsf_in_lane7  offset=slave bundle=gmem_in7  depth=4218750
    #pragma HLS INTERFACE m_axi port=sfd_out_lane0  offset=slave bundle=gmem_out0 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane1  offset=slave bundle=gmem_out1 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane2  offset=slave bundle=gmem_out2 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane3  offset=slave bundle=gmem_out3 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane4  offset=slave bundle=gmem_out4 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane5  offset=slave bundle=gmem_out5 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane6  offset=slave bundle=gmem_out6 depth=12656250
    #pragma HLS INTERFACE m_axi port=sfd_out_lane7  offset=slave bundle=gmem_out7 depth=12656250

    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane0  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane1  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane2  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane3  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane4  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane5  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane6  bundle=control
    #pragma HLS INTERFACE s_axilite port=tpsf_in_lane7  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane0  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane1  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane2  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane3  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane4  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane5  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane6  bundle=control
    #pragma HLS INTERFACE s_axilite port=sfd_out_lane7  bundle=control
    #pragma HLS INTERFACE s_axilite port=return         bundle=control

    #pragma HLS DATAFLOW

    lane_worker8(tpsf_in_lane0, sfd_out_lane0);
    lane_worker8(tpsf_in_lane1, sfd_out_lane1);
    lane_worker8(tpsf_in_lane2, sfd_out_lane2);
    lane_worker8(tpsf_in_lane3, sfd_out_lane3);
    lane_worker8(tpsf_in_lane4, sfd_out_lane4);
    lane_worker8(tpsf_in_lane5, sfd_out_lane5);
    lane_worker8(tpsf_in_lane6, sfd_out_lane6);
    lane_worker8(tpsf_in_lane7, sfd_out_lane7);
}