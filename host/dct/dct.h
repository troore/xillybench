#ifndef __DCT_H__
#define __DCT_H__

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>

//#define DW 16
#define DW 10000
//#define N 1024/DW
#define N 64
#define NUM_TRANS 16

#define D 4

typedef short dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

void dct(short input[N], short output[N]);
void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE]);
void read_data(short input[N], short buf[DCT_SIZE][DCT_SIZE]);
void write_data(short buf[DCT_SIZE][DCT_SIZE], short output[N]);
void dct_2d(short in_block[DCT_SIZE][DCT_SIZE],
			short out_block[DCT_SIZE][DCT_SIZE]);

void transpose(short in_block[DCT_SIZE][DCT_SIZE],
			   short out_block[DCT_SIZE][DCT_SIZE]);

void block_memcpy(short in_block[DCT_SIZE][DCT_SIZE],
				  short out_block[DCT_SIZE][DCT_SIZE]);

#endif // __DCT_H__ not defined
