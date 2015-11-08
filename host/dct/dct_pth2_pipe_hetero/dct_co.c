#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct.h"

void dct_1d(dct_data_t *src, dct_data_t *dst)
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "../dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}

void dct_2d(dct_data_t *in_block, dct_data_t *out_block)
{
	dct_data_t row_outbuf[DCT_SIZE * DCT_SIZE];
//	dct_data_t col_outbuf[DCT_SIZE][DCT_SIZE], col_inbuf[DCT_SIZE][DCT_SIZE];
	int fdr, fdw;
	unsigned i, j;
	int h;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	write(fdw, (void *)in_block, N * sizeof(dct_data_t));
	read(fdr, (void *)row_outbuf, N * sizeof(dct_data_t));

	// DCT rows
	/*
	for(i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block + i * DCT_SIZE, row_outbuf + i * DCT_SIZE);
	}
	*/
	for (h = 0; h < SFN; h++) {
	// Transpose data in order to re-use 1D DCT code
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++) {
		//	col_inbuf[j][i] = row_outbuf[i][j];
			out_block[j * DCT_SIZE + i] = row_outbuf[i * DCT_SIZE + j];
		}
	}

	close(fdr);
	close(fdw);
}

void read_data(dct_data_t *input, dct_data_t *buf)
{
	int r, c;
//	int h;

//	for (h = 0; h < SFN; h++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r * DCT_SIZE + c] = input[r * DCT_SIZE + c];
	}
//	}
}

void write_data(dct_data_t *buf, dct_data_t *output)
{
	int r, c;
//	int h;

//	for (h = 0; h < SFN; h++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r * DCT_SIZE + c];
	}
//	}
}

void dct(dct_data_t *input, dct_data_t *output)
{
	int i;
	dct_data_t buf_2d_in[DCT_SIZE * DCT_SIZE];
	dct_data_t buf_2d_out[DCT_SIZE * DCT_SIZE];

	for (i = 0; i < FF; i++) {
		// Read input data. Fill the internal buffer.
		read_data(input + i * N, buf_2d_in);

		dct_2d(buf_2d_in, buf_2d_out);

		// Write out the results.
		write_data(buf_2d_out, output + i * N);
	}
}

