#include "dct_2d.h"
#include "xilly_debug.h"

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int a[N], b[N];
//	int sfn, len;
	int i;

	xilly_puts("Hello, world!\n");

//	sfn = *in++;
//	len = sfn * DCT_SIZE * DCT_SIZE;
	for (i = 0; i < N; i++) {
		a[i] = (int)*in++;
	} 

//	half_dct_2d_test(a, b, sfn);
	half_dct_2d_test(a, b);

	for (i = 0; i < N; i++) {
	//	b[i] = 3;
//		b[i] = a[i];
		*out++ = (int)b[i];
	}
}
