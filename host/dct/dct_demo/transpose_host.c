#include <stdio.h>
#include "dct.h"
#include "timer.h"
#include "rapl_power.h"

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

