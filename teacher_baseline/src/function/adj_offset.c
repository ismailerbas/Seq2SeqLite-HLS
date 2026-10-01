#include <stdio.h>

void offset(const int size, const float  arr[size][1],const float min, float output[size][1]) {

    // Loop through the array and substract from the minimum value
    for (int i = 0; i < size; i++) {
#pragma HLS UNROLL

            output[i][0]=arr[i][0]-min;

    }


}
