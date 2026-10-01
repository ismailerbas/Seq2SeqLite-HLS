#include <stdio.h>
#include "findmax.h"

float findMax(int size, const float  arr[size][1]) {
   float  max_val = 0.0; // Initialize the max value

// Loop through the array to find the max value
    for (int i = 0; i < size; i++) {
#pragma HLS UNROLL
        if (arr[i][0] > max_val) {

        	max_val = arr[i][0];

        }
    }


    			return max_val;
}
