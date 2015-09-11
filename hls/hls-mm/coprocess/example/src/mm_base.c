#include "mm.h"

void mm_base(
      int a[N][N],
      int b[N][N],
      int c[N][N])
{
   int i, j, k;
  // Iterate over the rows of the A matrix
   Row: for(i = 0; i < N; i++) {
      // Iterate over the columns of the B matrix
      Col: for(j = 0; j < N; j++) {
			   int sum = 0;
         // Do the inner product of a row of A and col of B
         Product: for(k = 0; k < N; k++) {
            sum += a[i][k] * b[k][j];
         }
				  c[i][j] = sum;
      }
   }
}


