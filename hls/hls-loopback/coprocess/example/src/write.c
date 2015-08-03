#include <math.h>
#include <stdint.h>
#include "xilly_debug.h"

#define N	(1 << 16)

/*
int mycalc(int a, float *x2) {
	*x2 = sinf(*x2);
	return a + 1;
}
*/

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int i;

	xilly_puts("Hello, world\n");

	// Handle input data
//	n = *in++;

	// Debug output
	//  xilly_puts("x1=");
	//  xilly_decprint(x1, 1);
	//  xilly_puts("\n");

	// Run the calculations

	// Handle output data
	for (i = 0; i < N; i++)
	{
		int tmp;
		tmp = *in++;
		*out++ = tmp;
	}
}
