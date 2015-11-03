#ifndef __DCT_CO_H_
#define __DCT_CO_H_

// number of stage frames
#define SFN 1000000
#define FF 10
//#define N 1024/DW
#define N 64
#define NUM_TRANS 16

//typedef short dct_data_t;
typedef int dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

void dct_2d_test(dct_data_t *a, dct_data_t *b);

#endif // __DCT_H__ not defined
