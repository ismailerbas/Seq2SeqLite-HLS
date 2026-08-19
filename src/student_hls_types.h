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

// quantized_tanh output domain: KEPT 8 bit with round + saturate. The cast
// "state_t hh = state_t(sum_h);" in gru_cell_step is the hardware
// implementation of QKeras quantized_tanh(8): verified from
// qkeras/quantizers.py, quantized_tanh.__call__ computes
// 2*hard_sigmoid(x)-1 (== clip(x,-1,1) for the default "hard" internal
// sigmoid) then rounds to a 1/128 grid and clips into [-1, 127/128] --
// exactly the range and step of ap_fixed<8,1> with saturation.
// Rounding mode is AP_RND_CONV (round half to even), which bit-matches
// tf.round used inside QKeras's _round_through; plain AP_RND (round half
// toward +inf) differs from the model on exact-tie inputs.
// This type is used ONLY for the hh candidate value, because that is the
// ONLY place the trained model quantizes an activation in the GRU cell.
typedef ap_fixed<8, 1, AP_RND_CONV, AP_SAT>  state_t;

// Carried hidden-state domain: WIDE, deliberately NOT 8 bit. Verified from
// qkeras/qrecurrent.py QGRUCell.call: with state_quantizer=None (this
// model has none -- extraction_report.json contains only kernel /
// recurrent / bias quantizers), the carried state h is the raw float32
// blend h = z*h_tm1 + (1-z)*hh, and the reset product r*h_tm1 feeding the
// candidate matmul is also raw float. Quantizing the carried state to
// state_t on every timestep (as this design originally did) injects up to
// 2^-8 error per element per step, which compounds across the 135 encoder
// + 135 decoder recurrent steps into the 0.05-0.17 output errors observed
// in csim. 22 fractional bits give 2.4e-7 resolution -- float32-equivalent
// at |h| <= 1 -- and a 24-bit operand still fits the DSP48E1 25x18
// multiplier in a single DSP alongside the 8-bit weight, so the II=4 /
// cyclic-factor-8 timing architecture is unchanged.
typedef ap_fixed<24, 2, AP_TRN, AP_WRAP> hstate_t;

// Encoder raw input: normalized decay values in [0,1] (see eval_experimental.py
// load_and_preprocess_mat: per-pixel max-normalized, clamped >= 0). Extra
// integer bit gives headroom; input itself is NOT quantized by QKeras.
// Relaxed to AP_TRN/AP_WRAP: this is a MAC operand, not a quantization
// boundary, so no rounding/saturation logic is needed on every read.
typedef ap_fixed<16, 2, AP_TRN, AP_WRAP> input_t;

// Gate activations (hard_sigmoid output), bounded to [0,1], unquantized in
// the model (QKeras hard_sigmoid returns raw float). Widened from 16 to 18
// bits (16 fractional bits): gate values multiply the carried state every
// timestep, and AP_TRN truncation bias at 2^-14 could accumulate to
// milli-scale over 270 recurrent steps; at 2^-16 the worst-case
// accumulated bias is far below the 0.02 verification threshold. 18-bit
// gate x 24-bit state still fits one DSP48E1 (25x18). The hard_sigmoid
// clip is implemented explicitly with if/else comparisons in gru_cell_step,
// so the type itself does not need to saturate.
typedef ap_fixed<18, 2, AP_TRN, AP_WRAP> gate_t;

// Wide accumulator for MAC sums (kernel + recurrent + bias), avoids overflow
// across up to 32 accumulated products before any clipping/quantization.
// 22 fractional bits, so hstate_t operands cast into it exactly.
// Relaxed to AP_TRN/AP_WRAP: this is a pure intermediate accumulator with
// 10 integer bits of headroom for 32 accumulated products, so overflow
// saturation is not required, and rounding every partial sum is
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