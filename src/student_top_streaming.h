#ifndef STUDENT_TOP_STREAMING_H
#define STUDENT_TOP_STREAMING_H

#include "student_top.h"

#define NUM_LANES 4
#define TOTAL_PIXELS 250000
#define PIXELS_PER_LANE (TOTAL_PIXELS / NUM_LANES)

// Streaming, 4-lane, shared-compute frame inference top function.
// Each lane processes PIXELS_PER_LANE (62500) pixels sequentially, reusing
// its own single physical copy of student_infer_pixel across all of them.
// The 4 lanes run concurrently via HLS DATAFLOW. Frame data is split into
// 4 disjoint host-side buffers ahead of time (one per lane) -- see
// prepare_frame_lanes.py -- so each lane operates on its own m_axi port
// with no address overlap, which is required for the DATAFLOW scheduler
// to treat the 4 lanes as truly independent.
void student_infer_frame_streaming(
    input_t  *tpsf_in_lane0,
    input_t  *tpsf_in_lane1,
    input_t  *tpsf_in_lane2,
    input_t  *tpsf_in_lane3,
    output_t *sfd_out_lane0,
    output_t *sfd_out_lane1,
    output_t *sfd_out_lane2,
    output_t *sfd_out_lane3
);

#endif // STUDENT_TOP_STREAMING_H