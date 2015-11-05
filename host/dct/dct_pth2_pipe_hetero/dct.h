#ifndef __DCT_CO_H_
#define __DCT_CO_H_

//#define DW 16
//#define DW 10000
// number of frames in a superframe
#define SFN 100000
#define FF 10 // number of superframes
//#define N 1024/DW
#define N 64
#define NUM_TRANS 16

#define D 2

//typedef short dct_data_t;
typedef int dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

void dct(dct_data_t *input, dct_data_t *output);
void dct_1d(dct_data_t *src, dct_data_t *dst);
void read_data(dct_data_t *input, dct_data_t *buf);
void write_data(dct_data_t *buf, dct_data_t *output);
void dct_2d(dct_data_t *in_block, dct_data_t *out_block);

void transpose(dct_data_t *in_block, dct_data_t *out_block);

void block_memcpy(dct_data_t *in_block, dct_data_t *out_block);

#endif // __DCT_H__ not defined
