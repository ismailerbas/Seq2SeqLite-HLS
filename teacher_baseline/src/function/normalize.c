#include <stdio.h>
#include "findmax.h"

void norm(const int size, const float  arr[size][1],float output[size][1]) {
	float max=0.0;
	max=findMax(size,arr);

    // Loop through the array and normalize
    for (int i = 0; i < size; i++) {
#pragma HLS UNROLL

            output[i][0]=arr[i][0]/max;

    }


}
