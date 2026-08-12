#ifndef STUDENT_HLS_TYPES_H
#define STUDENT_HLS_TYPES_H

#include <ap_fixed.h>
#include <ap_int.h>

// Weight / bias / state domain: quantized_bits(8, 0, 1, alpha=1.0)
// integer=0 -> 1 sign bit + 7 fractional bits -> range [-1, 1), step 1/128.
typedef ap_fixed<8, 1, AP_RND, AP_SAT>  weight_t;
typedef ap_fixed<8, 1, AP_RND, AP_SAT>  bias_t;
typedef ap_fixed<8, 1, AP_RND, AP_SAT>  state_t;

// Encoder raw input: normalized decay values in [0,1] (see eval_experimental.py
// load_and_preprocess_mat: per-pixel max-normalized, clamped >= 0). Extra
// integer bit gives headroom; input itself is NOT quantized by QKeras.
typedef ap_fixed<16, 2, AP_RND, AP_SAT> input_t;

// Gate activations (hard_sigmoid output), bounded to [0,1], unquantized.
typedef ap_fixed<16, 2, AP_RND, AP_SAT> gate_t;

// Wide accumulator for MAC sums (kernel + recurrent + bias), avoids overflow
// across up to 32 accumulated products before any clipping/quantization.
typedef ap_fixed<32, 10, AP_RND, AP_SAT> acc_t;

// QDense output domain: kernel_quantizer = quantized_bits(8,0), same [-1,1)
// weight domain, but the matmul RESULT itself is NOT quantized (activation
// is "linear", no output quantizer attached in QDense for this model).
typedef ap_fixed<32, 10, AP_RND, AP_SAT> dense_acc_t;
typedef ap_fixed<16, 6, AP_RND, AP_SAT> output_t;

#define GRU_UNITS   32
#define INPUT_DIM   1
#define SEQ_LEN     135
#define N_OUT       3

#endif // STUDENT_HLS_TYPES_H