#include <math.h>
#include <stdint.h>
#include "xilly_debug.h"

#include "mm.h"

//#define OPT

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int i, j;
	int n;

	int a[N / 2][N], b[N][N], c[N / 2][N];

	xilly_puts("Hello, world.\n");

	//  n = *in++;

	// Handle input data
	for (i = 0; i < (N / 2); i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = *in++;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			b[i][j] = *in++;
		}
	}

	// Run the calculations
#ifndef OPT
	mm_base(a, b, c);
#else
	mm_opt(a, b, c);
#endif

	for (i = 0; i < (N / 2); i++) {
		for (j = 0; j < N; j++) {
			*out++ = c[i][j];
		}
	}
}
