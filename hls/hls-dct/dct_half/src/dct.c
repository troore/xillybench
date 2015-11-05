#include "dct.h"

void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
{
   unsigned int k, n;
   int tmp;
   const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "dct_coeff_table.txt"
   };

DCT_Outer_Loop:
   for (k = 0; k < DCT_SIZE; k++) {
#pragma HLS PIPELINE
DCT_Inner_Loop:
      for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
         int coeff = (int)dct_coeff_table[k][n];
         tmp += src[n] * coeff;
      }
      dst[k] = DESCALE(tmp, CONST_BITS);
   }
}

void read_data(dct_data_t input[N], dct_data_t buf[DCT_SIZE][DCT_SIZE])
{
   int r, c;
//  int h;

//   for (h = 0; h < SFN; h++) {
RD_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
RD_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
#pragma HLS PIPELINE
    	  buf[r][c] = input[r * DCT_SIZE + c];
   }
//   }
}

void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
		dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	dct_data_t row_outbuf[DCT_SIZE][DCT_SIZE];
//	dct_data_t col_outbuf[DCT_SIZE][DCT_SIZE], col_inbuf[DCT_SIZE][DCT_SIZE];
	unsigned i, j;
	int h;

	for (h = 0; h < SFN; h++) {
	// DCT rows
Row_DCT_Loop:
	for(i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], row_outbuf[i]);
	}
	}
	for (h = 0; h < SFN; h++) {
	// Transpose data in order to re-use 1D DCT code
Xpose_Row_Outer_Loop:
	for (j = 0; j < DCT_SIZE; j++)
		Xpose_Row_Inner_Loop:
			for(i = 0; i < DCT_SIZE; i++)
			{
#pragma HLS PIPELINE
			//	col_outbuf[j][i] = row_outbuf[i][j];
				out_block[j][i] = row_outbuf[i][j];
			}
	}
	/*
	// DCT columns
Col_DCT_Loop:
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(col_inbuf[i], col_outbuf[i]);
	}
	// Transpose data back into natural order
Xpose_Col_Outer_Loop:
	for (j = 0; j < DCT_SIZE; j++)
		Xpose_Col_Inner_Loop:
			for(i = 0; i < DCT_SIZE; i++)
				out_block[j][i] = col_outbuf[i][j];
	 */
}

void write_data(dct_data_t buf[DCT_SIZE][DCT_SIZE], dct_data_t output[N])
{
   int r, c;
//   int h;

//   for (h = 0; h < SFN; h++) {
WR_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
WR_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
#pragma HLS PIPELINE
    	  output[r * DCT_SIZE + c] = buf[r][c];
   }
//   }
}

void dct(dct_data_t input[N], dct_data_t output[N])
{
   dct_data_t buf_2d_in[DCT_SIZE][DCT_SIZE];
//#pragma HLS ARRAY_PARTITION variable=buf_2d_in complete dim=2
   dct_data_t buf_2d_out[DCT_SIZE][DCT_SIZE];

   // Read input data. Fill the internal buffer.
   read_data(input, buf_2d_in);

   //   dct_2d(buf_2d_in, buf_2d_out);
   dct_2d(buf_2d_in, buf_2d_out);

   // Write out the results.
   write_data(buf_2d_out, output);
}
