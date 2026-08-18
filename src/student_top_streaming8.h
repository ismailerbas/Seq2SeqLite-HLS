#ifndef STUDENT_TOP_STREAMING8_H
#define STUDENT_TOP_STREAMING8_H

#include "student_top.h"

#define NUM_LANES8 8
#define TOTAL_PIXELS8 250000
#define PIXELS_PER_LANE8 (TOTAL_PIXELS8 / NUM_LANES8)

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
);

#endif // STUDENT_TOP_STREAMING8_H