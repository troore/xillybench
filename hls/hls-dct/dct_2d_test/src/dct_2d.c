#include "dct_2d.h"

void dct_1d(dct_data_t *src, dct_data_t *dst)
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
#pragma HLS PIPELINE
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}

void half_dct_2d_test(dct_data_t *in_block, dct_data_t *out_block)
{
	int k, i;

	for (k = 0; k < SFN; k++) {
		for (i = 0; i < DCT_SIZE; i++) {
			dct_1d(in_block + i * DCT_SIZE, out_block + i * DCT_SIZE);
		}
	}
}

/*
void half_dct_2d_test(dct_data_t *in_block, dct_data_t *out_block)
{
	int k, i, j;
	int idx;

	for (k = 0; k < SFN; k++) {
		for (i = 0; i < DCT_SIZE; i++) {
			for (j = 0; j < DCT_SIZE; j++)
				idx = k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE + j;
				out_block[idx] = in_block[idx];
		}
	}
}
*/
