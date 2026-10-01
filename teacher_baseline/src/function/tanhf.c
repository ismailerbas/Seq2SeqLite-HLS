#include "tanhf.h"
#include <math.h>
float mytanh(float x) {
    // Calculate the hyperbolic tangent value using the exponential function
    // (e^x - e^-x) / (e^x + e^-x)
    float exp_pos = exp(x);
    float exp_neg = exp(-x);
    return (exp_pos - exp_neg) / (exp_pos + exp_neg);
}
