#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../function/sigmoid.h"
#include "../function/tanhf.h"
#include "grucell.h"
#include "../function/matmul.h"
#include "../function/innerproduct.h"
#include <string.h>


void gru_cell(int input_size, int hidden_size, int input_features, float cell_hidden[1][hidden_size] , const float input[1][input_features], const float weights_x[][hidden_size*3],const  float weights_h[][hidden_size*3], const float* bias1, const float* bias2)
{
	int i, j, t;
	// Encoder Layer
#define n1 128 //Number of hidden units
#define T1 70//Sequence length
#define d1 128 // Number of input features


	float W1_z[d1][n1], U1_z[n1][n1], b1_z[n1], bh_z[n1];
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=W1_z
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=U1_z
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=b1_z
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=bh_z

	float W1_r[d1][n1], U1_r[n1][n1], b1_r[n1], bh_r[n1];
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=W1_r
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=U1_r
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=b1_r
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=bh_r

	float W1_h[d1][n1], U1_h[n1][n1], b1_h[n1], bh_h[n1];
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=W1_h
#pragma HLS ARRAY_PARTITION dim=0 factor=32 type=block variable=U1_h
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=b1_h
#pragma HLS ARRAY_PARTITION dim=1 factor=32 type=block variable=bh_h

	float h1[1][n1];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=h1



	int T = input_size; // Sequence length
	int d = input_features; // Number of input features
	int n = hidden_size;//Number of hidden units


	// Extract weights and biases
	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		for (j = 0; j < d; j++) {
#pragma HLS UNROLL
			W1_z[j][i] = weights_x[j][i];
			W1_r[j][i] = weights_x[j][i+n];
			W1_h[j][i] = weights_x[j][i+2*n];
		}
		for (j = 0; j < n; j++) {

			U1_z[i][j] = weights_h[i][j];
			U1_r[i][j] = weights_h[i][j+n];
			U1_h[i][j] = weights_h[i][j+2*n];

		}
		b1_z[i] = bias1[i];
		b1_r[i] = bias1[n + i];
		b1_h[i] = bias1[2 * n + i];
		bh_r[i] = bias2[i];
		bh_z[i] = bias2[n + i];
		bh_h[i] = bias2[2 * n + i];
	}








	float result[1][n1];
	float resulth[1][n1];

	float resultreccurrent[1][n1];
#pragma HLS ARRAY_PARTITION dim=2 factor=32 type=block variable=result
#pragma HLS ARRAY_PARTITION dim=2 factor=32 type=block variable=resulth
#pragma HLS ARRAY_PARTITION dim=2 factor=32 type=block variable=resultreccurrent
	float x_t[1][d];






	// Update gate
	float z1_t[n];

	float r1_t[1][n];

	float h1_tilde[1][n];


	matrix_multiply( 1, d,n,  input  , W1_z,result);
	matrix_multiply( 1,  n,  n,  cell_hidden,  U1_z,  resultreccurrent);

	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		z1_t[i] = 1 / (1 + exp(result[0][i] +resultreccurrent[0][i] + b1_z[i] + bh_r[i]));
	}



	// Reset gate

	matrix_multiply( 1, d,n,  input  , W1_r,result);
	matrix_multiply( 1,  n,  n,  cell_hidden,  U1_r,  resultreccurrent);

	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		r1_t[0][i] = sigmoid(result[0][i] +resultreccurrent[0][i] + b1_r[i] + bh_z[i]);
	}

	// Candidate hidden state
	matrix_multiply(1, d,n,  input  , W1_h,result);
	matrix_multiply( 1,  n,  n,  cell_hidden,  U1_h,  resultreccurrent);
	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		h1_tilde[0][i] = resultreccurrent[0][i] + bh_h[i];
	}
	inner_product(1, n,  r1_t  , h1_tilde,resulth);

	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		h1_tilde[0][i] = tanhf(result[0][i] +resulth[0][i] + b1_h[i] );
	}

	//Updated hidden state

	for (i = 0; i < n; i++) {
#pragma HLS UNROLL
		cell_hidden[0][i] = (1 - z1_t[i]) * cell_hidden[0][i] + z1_t[i] * h1_tilde[0][i];

	}












}
//}
