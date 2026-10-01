#ifndef MATRIX_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_H

#define N 3

void matrix_multiply(int A_rows, int A_cols, int B_cols,const float A[A_rows][A_cols], const float B[A_cols][B_cols], float C[A_rows][B_cols]);

#endif /* MATRIX_MULTIPLICATION_H */
