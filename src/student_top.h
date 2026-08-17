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

// Post-processing: extract per-pixel lifetimes (tau1, tau2) and FRET
// fraction from a completed sfd_out sequence, replicating
// extract_lifetimes() from eval_experimental.py exactly:
//   int1 = trapz(sfd_out[:,1], t)   channel 1 = short-lifetime component
//   int2 = trapz(sfd_out[:,2], t)   channel 2 = long-lifetime component
//   amp1 = sfd_out[0][1], amp2 = sfd_out[0][2]
//   tau1 = int1/amp1 if amp1 > 1e-6 else 0.0
//   tau2 = int2/amp2 if amp2 > 1e-6 else 0.0
//   fret = amp1/(amp1+amp2) if (amp1+amp2) > 1e-6 else 0.5
// t is the time axis in ns: t[k] = k * GATE_WIDTH_NS.
void extract_lifetimes_pixel(
    output_t sfd_out[SEQ_LEN][N_OUT],
    double &tau1,
    double &tau2,
    double &fret
);

#endif // STUDENT_TOP_H