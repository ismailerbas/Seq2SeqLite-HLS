#ifndef STUDENT_HLS_TYPES_H
#define STUDENT_HLS_TYPES_H

#include <ap_fixed.h>
#include <ap_int.h>

// Weight / bias domain: quantized_bits(8, 0, 1, alpha=1.0)
// integer=0 -> 1 sign bit + 7 fractional bits -> range [-1, 1), step 1/128.
// AP_TRN/AP_WRAP here is safe: weight_t and bias_t are only ever assigned
// exact int8-code conversions (int8_to_weight/int8_to_bias), which already
// land exactly on a representable 1/128 step -- there is nothing to round
// or saturate, so AP_RND/AP_SAT on these types was pure wasted combinational
// logic (rounding adder + saturation comparator) on every single MAC.
typedef ap_fixed<8, 1, AP_TRN, AP_WRAP>  weight_t;
typedef ap_fixed<8, 1, AP_TRN, AP_WRAP>  bias_t;

// State domain: KEPT AP_RND, AP_SAT. This is not a timing-cost typedef --
// "state_t hh = state_t(sum_h);" in gru_cell_step is the actual hardware
// implementation of quantized_tanh's clip(-1,1) + round-to-1/128 behavior
// from the trained QKeras model. Changing this to AP_TRN/AP_WRAP would
// silently change model accuracy (wrap-around overflow instead of clip,
// and truncation instead of round-to-nearest), so it must not be touched.
typedef ap_fixed<8, 1, AP_RND, AP_SAT>  state_t;

// Encoder raw input: normalized decay values in [0,1] (see eval_experimental.py
// load_and_preprocess_mat: per-pixel max-normalized, clamped >= 0). Extra
// integer bit gives headroom; input itself is NOT quantized by QKeras.
// Relaxed to AP_TRN/AP_WRAP: this is a MAC operand, not a quantization
// boundary, so no rounding/saturation logic is needed on every read.
typedef ap_fixed<16, 2, AP_TRN, AP_WRAP> input_t;

// Gate activations (hard_sigmoid output), bounded to [0,1], unquantized.
// Relaxed to AP_TRN/AP_WRAP: the hard_sigmoid clip is already implemented
// explicitly with if/else comparisons in gru_cell_step, so the type itself
// does not need to saturate -- the clip logic in the C code is what matters.
typedef ap_fixed<16, 2, AP_TRN, AP_WRAP> gate_t;

// Wide accumulator for MAC sums (kernel + recurrent + bias), avoids overflow
// across up to 32 accumulated products before any clipping/quantization.
// Relaxed to AP_TRN/AP_WRAP: this is a pure intermediate accumulator with
// 10 integer bits of headroom for 32 accumulated 8x8-bit products, so
// overflow saturation is not required, and rounding every partial sum is
// unnecessary precision that was costing timing on the reset-gate multiply.
typedef ap_fixed<32, 10, AP_TRN, AP_WRAP> acc_t;

// QDense output domain: kernel_quantizer = quantized_bits(8,0), same [-1,1)
// weight domain, but the matmul RESULT itself is NOT quantized (activation
// is "linear", no output quantizer attached in QDense for this model).
// dense_acc_t relaxed for the same reason as acc_t above. output_t is KEPT
// AP_RND, AP_SAT because it is the final value written to sfd_out and must
// round/saturate correctly as the last step before leaving the function.
typedef ap_fixed<32, 10, AP_TRN, AP_WRAP> dense_acc_t;
typedef ap_fixed<16, 6, AP_RND, AP_SAT>  output_t;

#define GRU_UNITS   32
#define INPUT_DIM   1
#define SEQ_LEN     135
#define N_OUT       3

// Gate width per time bin in nanoseconds. Confirmed from both
// train_student_vanilla_kd.py and eval_experimental.py --gate-width-ns
// default (0.09). Used for trapezoidal lifetime integration in
// extract_lifetimes_pixel (student_top.cpp), matching extract_lifetimes()
// in eval_experimental.py exactly.
#define GATE_WIDTH_NS 0.09

#endif // STUDENT_HLS_TYPES_H
