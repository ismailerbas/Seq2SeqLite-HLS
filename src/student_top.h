#ifndef STUDENT_TOP_H
#define STUDENT_TOP_H

#include "student_hls_types.h"

// Single-pixel Seq2SeqLite student inference.
// tpsf_in  : normalized TPSF sequence for one pixel, length SEQ_LEN.
// sfd_out  : reconstructed SFD sequence, SEQ_LEN x N_OUT
//            (channel 0 = full decay, channel 1 = short, channel 2 = long).
void student_infer_pixel(
    input_t  tpsf_in[SEQ_LEN],
    output_t sfd_out[SEQ_LEN][N_OUT]
);

#endif // STUDENT_TOP_H