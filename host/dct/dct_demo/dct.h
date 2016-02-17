#ifndef __DCT_PROF_H__
#define __DCT_PORF_H__

#define N 64

//typedef dct_data_t dct_data_t;
typedef int dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

void dct(dct_data_t input[N], dct_data_t output[N]);
void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE]);
void read_data(dct_data_t input[N], dct_data_t buf[DCT_SIZE][DCT_SIZE]);
void write_data(dct_data_t buf[DCT_SIZE][DCT_SIZE], dct_data_t output[N]);
void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE]);

void transpose(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			   dct_data_t out_block[DCT_SIZE][DCT_SIZE]);

#endif // __
