#ifndef __DCT_H_
#define __DCT_H_

#define N 64

//typedef dct_data_t dct_data_t;
typedef int dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

//void dct_wrapper(dct_data_t input[N], dct_data_t output[N]);

//void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
//			dct_data_t out_block[DCT_SIZE][DCT_SIZE]);

//void transpose(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
//			   dct_data_t out_block[DCT_SIZE][DCT_SIZE]);

#endif // __
