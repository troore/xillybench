#ifndef __DCT_HOST_H_
#define __DCT_HOST_H_

void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE]);

void transpose(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			   dct_data_t out_block[DCT_SIZE][DCT_SIZE]);


#endif
