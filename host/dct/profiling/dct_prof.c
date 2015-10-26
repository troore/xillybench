#include <stdio.h>
#include "dct_prof.h"
#include "timer.h"

void dct_1d_prof(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
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

void dct_2d_prof(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int i;

	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d_prof(in_block[i], out_block[i]);
	}
}

void transpose_prof(dct_data_t in_block[DCT_SIZE][DCT_SIZE], 
		dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;

	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			out_block[j][i] = in_block[i][j];
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


void dct_prof(short input[N], short output[N])
{
	int i;
	short buf_2d_in[DCT_SIZE][DCT_SIZE];
	short buf_2d_buf[DCT_SIZE][DCT_SIZE];
	short buf_2d_out[DCT_SIZE][DCT_SIZE];

	double start, finish, elapsed_time;

	read_data(input, buf_2d_in);

//	start = dtime();

	for (i = 0; i < DW; i++) {
		dct_2d_prof(buf_2d_in, buf_2d_buf);
	}

//	finish = dtime();
//	elapsed_time = finish - start;
	start = dtime();
	for (i = 0; i < DW; i++) {
		transpose_prof(buf_2d_buf, buf_2d_out);
	}
	finish = dtime();
	elapsed_time = finish - start;
	write_data(buf_2d_out, output);

	printf("%.3fms\n", elapsed_time);
}
