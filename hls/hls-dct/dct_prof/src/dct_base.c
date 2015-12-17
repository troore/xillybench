#include "dct.h"

void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
{
   unsigned int k, n;
   int tmp;
   const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
  8192,  8192,  8192,  8192,  8192,  8192,  8192,  8192,
 11363,  9633,  6436,  2260, -2260, -6436, -9632,-11362,
 10703,  4433, -4433,-10703,-10703, -4433,  4433, 10703,
  9633, -2260,-11362, -6436,  6436, 11363,  2260, -9632,
  8192, -8192, -8192,  8192,  8192, -8191, -8191,  8192,
  6436,-11362,  2260,  9633, -9632, -2260, 11363, -6436,
  4433,-10703, 10703, -4433, -4433, 10703,-10703,  4433,
  2260, -6436,  9633,-11362, 11363, -9632,  6436, -2260
   };

DCT_Outer_Loop:
   for (k = 0; k < DCT_SIZE; k++) {
DCT_Inner_Loop:
      for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
         int coeff = (int)dct_coeff_table[k][n];
         tmp += src[n] * coeff;
      }
      dst[k] = DESCALE(tmp, CONST_BITS);
   }
}

void dct(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
      dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
//#pragma HLS INLINE
   dct_data_t row_outbuf[DCT_SIZE][DCT_SIZE];
   dct_data_t col_outbuf[DCT_SIZE][DCT_SIZE];
   dct_data_t col_inbuf[DCT_SIZE][DCT_SIZE];

   dct_2d(in_block, row_outbuf);

   transpose(row_outbuf, col_inbuf);

   dct_2d(col_inbuf, col_outbuf);

   transpose(col_outbuf, out_block);
}

void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
      dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	unsigned i;

Row_DCT_Loop:
	for(i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], out_block[i]);
	}
}

void transpose(dct_data_t in_block[DCT_SIZE][DCT_SIZE], 
      dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;
	// Transpose data back into natural order
Xpose_Col_Outer_Loop:
	for (j = 0; j < DCT_SIZE; j++)
		Xpose_Col_Inner_Loop:
			for(i = 0; i < DCT_SIZE; i++)
				out_block[j][i] = in_block[i][j];
}

void read_data(short input[N], short buf[DCT_SIZE][DCT_SIZE])
{
   int r, c;

RD_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
RD_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
         buf[r][c] = input[r * DCT_SIZE + c];
   }
}

void write_data(short buf[DCT_SIZE][DCT_SIZE], short output[N])
{
   int r, c;

WR_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
WR_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
         output[r * DCT_SIZE + c] = buf[r][c];
   }
}

void dct_test(short input[N], short output[N])
{
   short buf_2d_in[DCT_SIZE][DCT_SIZE];
   short buf_2d_out[DCT_SIZE][DCT_SIZE];

   // Read input data. Fill the internal buffer.
   read_data(input, buf_2d_in);

   dct(buf_2d_in, buf_2d_out);

   // Write out the results.
   write_data(buf_2d_out, output);
}

