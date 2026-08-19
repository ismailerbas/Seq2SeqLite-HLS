#ifndef STUDENT_TOP_INTERLEAVED_H
#define STUDENT_TOP_INTERLEAVED_H

#include "student_hls_types.h"

// ============================================================================
// Pixel-interleaved (multi-context) Seq2SeqLite engine.
//
// One physical GRU datapath shared by NPIX_ILV independent pixel contexts.
// The pipeline runs over CONTEXTS, not timesteps: by the time context p
// returns for timestep t+1, its timestep-t state was written NPIX_ILV
// pipeline iterations earlier, so the recurrent dependency never stalls the
// datapath. This is the RTL realization of the fine-grained shared-resource
// execution regime characterized by the STOMP analysis, as opposed to the
// coarse-grained lane replication of student_top_streaming{,8}.
// ============================================================================

// Number of pixel contexts sharing the datapath. 64 separates the recurrent
// write->read of each context by NPIX_ILV * ILV_II = 256 cycles at the
// default II, far beyond the pipeline depth, while keeping the replicated
// state footprint (the Bpixel term) small: 64 contexts x 32 units.
#define NPIX_ILV 64

// Initiation interval of the context pipeline. II=4 with the cyclic
// factor=8 recurrent-weight banking issues 32 units x 8 weights x 3 gates
// = 768 recurrent MACs per cycle (~790-830 DSP estimated on XC7K410T).
// If csynth cannot close II=4 or resources exceed budget, set to 8 and
// re-run: DSP demand halves, frame time doubles.
#define ILV_II 4

// Full-frame pixel count (500 x 500, SwissSPAD3 spatial format).
#define ILV_TOTAL_PIXELS 250000

// Core batch engine: processes NPIX_ILV pixels concurrently through the
// complete encoder -> decoder -> dense workload. Array interface so csim
// can drive it directly and compare bit-exactly against student_infer_pixel.
void student_infer_batch_interleaved(
    input_t  tpsf_in[NPIX_ILV][SEQ_LEN],
    output_t sfd_out[NPIX_ILV][SEQ_LEN][N_OUT]
);

// Full-frame top for synthesis: streams ILV_TOTAL_PIXELS pixels from DDR
// through the batch engine in ceil(ILV_TOTAL_PIXELS / NPIX_ILV) batches.
// Layouts match prepare_frame_lanes.py / decode_frame_lanes.py:
//   tpsf_frame_in : [pixel][t]            flat, ILV_TOTAL_PIXELS * SEQ_LEN
//   sfd_frame_out : [pixel][t][channel]   flat, ILV_TOTAL_PIXELS * SEQ_LEN * N_OUT
void student_infer_frame_interleaved(
    const input_t *tpsf_frame_in,
    output_t      *sfd_frame_out
);

#endif // STUDENT_TOP_INTERLEAVED_H