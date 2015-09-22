#include "dct.h"

void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}

void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	dct_data_t row_outbuf[DCT_SIZE][DCT_SIZE];
	dct_data_t col_outbuf[DCT_SIZE][DCT_SIZE], col_inbuf[DCT_SIZE][DCT_SIZE];
	unsigned i, j;

	// DCT rows
	for(i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], row_outbuf[i]);
	}
	// Transpose data in order to re-use 1D DCT code
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			col_inbuf[j][i] = row_outbuf[i][j];
	// DCT columns
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(col_inbuf[i], col_outbuf[i]);
	}
	// Transpose data back into natural order
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			out_block[j][i] = col_outbuf[i][j];
}

void read_data(short input[N], short buf[DCT_SIZE][DCT_SIZE])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r][c] = input[r * DCT_SIZE + c];
	}
}

void write_data(short buf[DCT_SIZE][DCT_SIZE], short output[N])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r][c];
	}
}

void dct(short input[DW * N], short output[DW * N])
{
	int i;
	short buf_2d_in[DCT_SIZE][DCT_SIZE];
	short buf_2d_out[DCT_SIZE][DCT_SIZE];

	for (i = 0; i < DW; i++) {
		// Read input data. Fill the internal buffer.
		read_data(input + i * N, buf_2d_in);

		dct_2d(buf_2d_in, buf_2d_out);

		// Write out the results.
		write_data(buf_2d_out, output + i * N);
	}
}

