#include "dct.h"
#include "xilly_debug.h"

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int a[N], b[N];
	int i;
	int h;

	xilly_puts("Hello, world!\n");

	for (i = 0; i < N; i++) {
		a[i] = (int)*in++;  // convert unit32_t to short 
	} 

	dct(a, b);

	for (i = 0; i < N; i++) {
		*out++ = (int)b[i];  // convert short to int
	}
}
