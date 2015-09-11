
#include "mm.h"

void mm_opt(
		int a[N][N],
		int b[N][N],
		int c[N][N])
{
#pragma HLS ARRAY_RESHAPE variable=b complete dim=1
#pragma HLS ARRAY_RESHAPE variable=a complete dim=2
	int a_row[N];
	int b_copy[N][N];
	int tmp = 0;
	int i, j, k;

	// Iterate over the rowa of the A matrix
Row: for (i = 0; i < N; i++) {
		 // Iterate over the columns of the B matrix
Col: for (j = 0; j < N; j++) {
#pragma HLS PIPELINE
		 // Do the inner product of a row of A and col of B
		 tmp = 0;
		 // Cache each row (so it's only read once per function)
		 if (j == 0)
			 Cache_Row: for (k = 0; k < N; k++)
				 a_row[k] = a[i][k];

		 // Cache all cols (so they are only read once per function)
		 if (i == 0)
			 Cache_Col: for (k = 0; k < N; k++)
				 b_copy[k][j] = b[k][j];

Product: for (k = 0; k < N; k++) {
			 tmp += a_row[k] * b_copy[k][j];
		 }
		 c[i][j] = tmp;
	 }
	 }
}

