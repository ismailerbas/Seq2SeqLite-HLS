

#include <stdio.h>
#include "matmul.h"

#include <stdio.h>

void inner_product(int rows, int cols, float A[rows][cols], float B[rows][cols],float result[rows][cols]) {
    int i, j;

    // Compute inner product
    for (i = 0; i < rows; i++) {
#pragma HLS UNROLL
        for (j = 0; j < cols; j++) {
#pragma HLS UNROLL
            result[i][j] = A[i][j] * B[i][j];

        }
    }


}
