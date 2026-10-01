#include <stdio.h>
#include "min.h"

float findMin(int size, const float  arr[size][1]) {
   float  min = 9999.0; // Initialize the minimum value

// Loop through the array to find the minimum value
    for (int i = 0; i < size; i++) {
#pragma HLS UNROLL
        if (arr[i][0] < min) {

           min = arr[i][0];

        }
    }


    			return min;
}
