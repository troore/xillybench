#include <math.h>
#include <stdint.h>

#define MAT_A_ROWS 3
#define MAT_A_COLS 3
#define MAT_B_ROWS 3
#define MAT_B_COLS 3

void matrixmul_baseline(
      int a[MAT_A_ROWS][MAT_A_COLS],
      int b[MAT_B_ROWS][MAT_B_COLS],
      int res[MAT_A_ROWS][MAT_B_COLS])
{
   int i,j,k;
  // Iterate over the rows of the A matrix
   Row: for(i = 0; i < MAT_A_ROWS; i++) {
      // Iterate over the columns of the B matrix
      Col: for(j = 0; j < MAT_B_COLS; j++) {
         res[i][j] = 0;
         // Do the inner product of a row of A and col of B
         Product: for(k = 0; k < MAT_B_ROWS; k++) {
            res[i][j] += a[i][k] * b[k][j];
         }
      }
   }

}

void matrixmul_opt(
      int a[MAT_A_ROWS][MAT_A_COLS],
      int b[MAT_B_ROWS][MAT_B_COLS],
      int res[MAT_A_ROWS][MAT_B_COLS])
{
#pragma HLS ARRAY_RESHAPE variable=b complete dim=1
#pragma HLS ARRAY_RESHAPE variable=a complete dim=2
 int a_row[MAT_A_ROWS];
  int b_copy[MAT_B_ROWS][MAT_B_COLS];
  int tmp = 0;
  int i,j,k;

  // Iterate over the rowa of the A matrix
  Row: for(i = 0; i < MAT_A_ROWS; i++) {
    // Iterate over the columns of the B matrix
    Col: for(j = 0; j < MAT_B_COLS; j++) {
#pragma HLS PIPELINE
      // Do the inner product of a row of A and col of B
      tmp=0;
      // Cache each row (so it's only read once per function)
      if (j == 0)
        Cache_Row: for(k = 0; k < MAT_A_ROWS; k++)
          a_row[k] = a[i][k];

       // Cache all cols (so they are only read once per function)
     if (i == 0)
            Cache_Col: for(k = 0; k < MAT_B_ROWS; k++)
               b_copy[k][j] = b[k][j];

      Product: for(k = 0; k < MAT_B_ROWS; k++) {
        tmp += a_row[k] * b_copy[k][j];
      }
      res[i][j] = tmp;
    }
  }
}



void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

  int i, j, k, tmp;

  int sel[MAT_A_ROWS][MAT_A_COLS], a[MAT_A_ROWS][MAT_A_COLS], b[MAT_B_ROWS][MAT_B_COLS], c[MAT_A_ROWS][MAT_A_COLS];


  // Handle input data
  for (i = 0; i < MAT_A_ROWS; i++){
	  for (j = 0; j < MAT_A_COLS; j++)
	  {
		   sel[i][j] = *in++;
		   a[i][j] = *in++;
		   b[i][j] = *in++;
	  }
  }


  // Run the calculations
  if ( sel[0][0] == 0 )
  {
      matrixmul_baseline(a, b, c);
  }
  else if ( sel[0][0] == 1 )
  {
      matrixmul_opt(a, b, c);
  }
  else;


  for (i = 0; i < MAT_A_ROWS; i++){
	  for (j = 0; j < MAT_A_COLS; j++) {
		  *out++ = c[i][j];
          }
  }
}
