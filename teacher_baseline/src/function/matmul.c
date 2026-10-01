

#include <stdio.h>
#include "matmul.h"



void matrix_multiply(int A_rows, int A_cols, int B_cols,const float A[A_rows][A_cols],const float B[A_cols][B_cols], float C[A_rows][B_cols]) {
	int i, j, k;

	float temp;
	// Matrix multiplication
	for (i = 0; i < A_rows; i++) {
#pragma HLS UNROLL
		for (j = 0; j < B_cols; j++) {
#pragma HLS UNROLL
			temp = 0;
			for (k = 0; k < A_cols; k++) {
#pragma HLS UNROLL
				temp += A[i][k] * B[k][j];


			}

			C[i][j] = temp;

		}
	}
}
