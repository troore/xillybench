#include <stdint.h>
#include "OFDM.h"
#include "lte_phy.h"
#include "xilly_debug.h"

#define NUM_RX_ANTENNA 2
#define N_OFDEMOD_IN	(NUM_RX_ANTENNA * (LTE_PHY_FFT_SIZE_1_92MHZ + LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ) * LTE_PHY_N_SYMB_PER_SUBFR)
#define N_OFDEMOD_OUT	(NUM_RX_ANTENNA * LTE_PHY_FFT_SIZE_1_92MHZ  * LTE_PHY_N_SYMB_PER_SUBFR)

#define N (2 * N_OFDEMOD_IN)
#define M (2 * N_OFDEMOD_OUT)

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
	//	*out++ = *((int *)(&b[i])); // synthesis fail
		tmp2 = b[i];
		*out++ = *((int *)(&tmp2));
	}
}
