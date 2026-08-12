#ifndef STUDENT_WEIGHT_CONVERT_H
#define STUDENT_WEIGHT_CONVERT_H

#include "student_hls_types.h"
#include <stdint.h>

// Bit-reinterpret an int8 quantized_bits(8,0,...) code as its ap_fixed<8,1>
// value. This is exact and lossless: the int8 two's-complement bit pattern
// IS the fixed-point representation with 1 integer (sign) bit and 7
// fractional bits, by construction of quantized_bits with integer=0.
inline weight_t int8_to_weight(int8_t raw) {
    weight_t w;
    ap_int<8> bits = raw;
    w.range(7, 0) = bits.range(7, 0);
    return w;
}

inline bias_t int8_to_bias(int8_t raw) {
    bias_t b;
    ap_int<8> bits = raw;
    b.range(7, 0) = bits.range(7, 0);
    return b;
}

#endif // STUDENT_WEIGHT_CONVERT_H