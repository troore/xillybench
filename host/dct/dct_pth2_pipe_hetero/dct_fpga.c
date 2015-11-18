#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct.h"

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

	int fdr, fdw;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	for (i = 0; i < FF; i++) {
		read_data(input + i * N, buf_2d_in);

		write(fdw, (void *)input, N * sizeof(dct_data_t));
		read(fdr, (void *)output, N * sizeof(dct_data_t));

		write_data(buf_2d_out, output + i * N);
	}

	close(fdr);
	close(fdw);
}
