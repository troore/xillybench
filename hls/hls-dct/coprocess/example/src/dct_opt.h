#ifndef DW
#define DW 16
#define N 1024/DW
#define NUM_TRANS 16
#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)
#endif

typedef short dct_data_t_opt;

void dct_1d_opt(dct_data_t_opt src[DCT_SIZE], dct_data_t_opt dst[DCT_SIZE])
{
   unsigned int k, n;
   int tmp;
   const dct_data_t_opt dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
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
#pragma HLS PIPELINE
DCT_Inner_Loop:
      for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
         int coeff = (int)dct_coeff_table[k][n];
         tmp += src[n] * coeff;
      }
      dst[k] = DESCALE(tmp, CONST_BITS);
   }
}

void dct_2d_opt(dct_data_t_opt in_block[DCT_SIZE][DCT_SIZE],
      dct_data_t_opt out_block[DCT_SIZE][DCT_SIZE])
{
#pragma HLS INLINE
   dct_data_t_opt row_outbuf[DCT_SIZE][DCT_SIZE];
   dct_data_t_opt col_outbuf[DCT_SIZE][DCT_SIZE];
   dct_data_t_opt col_inbuf[DCT_SIZE][DCT_SIZE];
#pragma HLS ARRAY_PARTITION variable=col_inbuf complete dim=2
   unsigned i, j;

   // DCT rows
Row_DCT_Loop:
   for(i = 0; i < DCT_SIZE; i++) {
      dct_1d_opt(in_block[i], row_outbuf[i]);
   }
   // Transpose data in order to re-use 1D DCT code
Xpose_Row_Outer_Loop:
   for (j = 0; j < DCT_SIZE; j++)
Xpose_Row_Inner_Loop:
      for(i = 0; i < DCT_SIZE; i++)
#pragma HLS PIPELINE
    	  col_inbuf[j][i] = row_outbuf[i][j];
   // DCT columns
Col_DCT_Loop:
   for (i = 0; i < DCT_SIZE; i++) {
      dct_1d_opt(col_inbuf[i], col_outbuf[i]);
   }
   // Transpose data back into natural order
Xpose_Col_Outer_Loop:
   for (j = 0; j < DCT_SIZE; j++)
Xpose_Col_Inner_Loop:
      for(i = 0; i < DCT_SIZE; i++)
#pragma HLS PIPELINE
    	  out_block[j][i] = col_outbuf[i][j];
}

void read_data_opt(short input[N], short buf[DCT_SIZE][DCT_SIZE])
{
   int r, c;

RD_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
RD_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
#pragma HLS PIPELINE
    	  buf[r][c] = input[r * DCT_SIZE + c];
   }
}

void write_data_opt(short buf[DCT_SIZE][DCT_SIZE], short output[N])
{
   int r, c;

WR_Loop_Row:
   for (r = 0; r < DCT_SIZE; r++) {
WR_Loop_Col:
      for (c = 0; c < DCT_SIZE; c++)
#pragma HLS PIPELINE
    	  output[r * DCT_SIZE + c] = buf[r][c];
   }
}

void dct_opt(short input[N], short output[N])
{
   short buf_2d_in[DCT_SIZE][DCT_SIZE];
#pragma HLS ARRAY_PARTITION variable=buf_2d_in complete dim=2
   short buf_2d_out[DCT_SIZE][DCT_SIZE];

   // Read input data. Fill the internal buffer.
   read_data_opt(input, buf_2d_in);

   dct_2d_opt(buf_2d_in, buf_2d_out);

   // Write out the results.
   write_data_opt(buf_2d_out, output);
}
