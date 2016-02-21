#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct.h"
#include "dct_host.h"
#include "dct_co.h"
#include "timer.h"
#include "rapl_power.h"

static void read_data(dct_data_t input[N], dct_data_t buf[DCT_SIZE][DCT_SIZE])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r][c] = input[r * DCT_SIZE + c];
	}
}

static void write_data(dct_data_t buf[DCT_SIZE][DCT_SIZE], dct_data_t output[N])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r][c];
	}
}


void dct_wrapper(dct_data_t input[N], dct_data_t output[N])
{
	double start, finish, elapsed_time;

	dct_data_t buf_2d_in[DCT_SIZE][DCT_SIZE];
	dct_data_t buf_2d_buf[DCT_SIZE][DCT_SIZE];
	dct_data_t buf_2d_out[DCT_SIZE][DCT_SIZE];

	read_data(input, buf_2d_in);

#ifndef CO
	dct_2d(buf_2d_in, buf_2d_buf);

	transpose(buf_2d_buf, buf_2d_out);
#else
	start = dtime();
	dct_2d_co(buf_2d_in, buf_2d_buf);
	finish = dtime();
	elapsed_time = finish - start;
	printf("time_dct_2d %.3f s\n", elapsed_time / 1000.0);

	transpose(buf_2d_buf, buf_2d_out);
#endif

	write_data(buf_2d_out, output);
}
