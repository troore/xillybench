#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct.h"
#include "timer.h"
#include "rapl_power.h"

void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
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

void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int fdr, fdw;
	int nw, nr;

	dct_data_t *a, *b;

	a = (dct_data_t *)in_block;
	b = (dct_data_t *)out_block;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	nw = write(fdw, (void *)a, N * sizeof(dct_data_t));
//	printf("%d\n", nw);
	nr = read(fdr, (void *)b, N * sizeof(dct_data_t));
//	printf("%d\n", nr);

	close(fdr);
	close(fdw);
}

void transpose(dct_data_t in_block[DCT_SIZE][DCT_SIZE], 
		dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;
	int h;

	for (h = 0; h < 100000; h++) {
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			out_block[j][i] = in_block[i][j];
	}
}

void read_data(dct_data_t input[N], dct_data_t buf[DCT_SIZE][DCT_SIZE])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r][c] = input[r * DCT_SIZE + c];
	}
}

void write_data(dct_data_t buf[DCT_SIZE][DCT_SIZE], dct_data_t output[N])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r][c];
	}
}


void dct(dct_data_t input[N], dct_data_t output[N])
{

	double start, finish, elapsed_time;

	dct_data_t buf_2d_in[DCT_SIZE][DCT_SIZE];
	dct_data_t buf_2d_buf[DCT_SIZE][DCT_SIZE];
	dct_data_t buf_2d_out[DCT_SIZE][DCT_SIZE];

	read_data(input, buf_2d_in);


	start = dtime();
	dct_2d(buf_2d_in, buf_2d_buf);
	finish = dtime();
	elapsed_time = finish - start;
	printf("dct_2d %.3f ms\n", elapsed_time);

	transpose(buf_2d_buf, buf_2d_out);
	write_data(buf_2d_out, output);
}
