#ifndef STUDENT_TOP_PARALLEL_H
#define STUDENT_TOP_PARALLEL_H

#include "student_top.h"

#define NUM_PARALLEL_PIXELS 256

// 256-way spatially-parallel batch inference, for csynth resource
// utilization measurement only. Each of the 256 pixels gets its own
// fully independent instance of student_infer_pixel -- this is NOT a
// time-multiplexed/streaming design, it is 256 complete physical copies
// of the entire single-pixel datapath (encoder GRU + decoder GRU + dense
// head), each with its own DSP48s, LUTs, and weight storage.
void student_infer_batch_parallel(
    input_t  tpsf_in_batch[NUM_PARALLEL_PIXELS][SEQ_LEN],
    output_t sfd_out_batch[NUM_PARALLEL_PIXELS][SEQ_LEN][N_OUT]
);

#endif // STUDENT_TOP_PARALLEL_H