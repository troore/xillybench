#include <stdint.h>
#include "OFDM.h"
#include "lte_phy.h"
#include "xilly_debug.h"

#define N (2 * N_OFDEMOD_IN_MAX)
#define M (2 * N_OFDEMOD_OUT_MAX)

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	float a[N], b[M];
	uint32_t tmp1;
	float tmp2;
	int i;

	xilly_puts("Hello, world!\n");

	for (i = 0; i < N; i++) {
		tmp1 = *in++;
		a[i] = *((float *)(&tmp1));
	} 

	ofdemodulating(a, b);

	for (i = 0; i < M; i++) {
		tmp2 = b[i];
		*out++ = *((int *)(&tmp2));
	}
}
