
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fft.h"
#include "lte_phy.h"

#define NUM_RX_ANTENNA 2
#define NUM_UL_SYMB_SF LTE_PHY_N_SYMB_PER_SUBFR
#define NUM_FFT LTE_PHY_FFT_SIZE_30_72MHZ
#define N_OFDEMOD_IN	(NUM_RX_ANTENNA * (NUM_FFT + LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ) * NUM_UL_SYMB_SF)
#define N_OFDEMOD_OUT	(NUM_RX_ANTENNA * NUM_FFT * NUM_UL_SYMB_SF)

void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData)
{
	int fdr, fdw;
	int nr, nw;
	unsigned int donebytes;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	// Not checking return values of write() and read(). This must be done
	// in a real-life program to ensure reliability.
	nw = write(fdw, (void *)pInpData, (2 * N_OFDEMOD_IN) * sizeof(float));
	printf("%d\n", nw);
	donebytes = 0;
	while (donebytes < 2 * N_OFDEMOD_OUT * sizeof(float)) {
		nr = read(fdr, pOutData + donebytes, 2 * N_OFDEMOD_OUT * sizeof(float) - donebytes);
		printf("%d\n", nr);
		if ((nr < 0) && errno == EINTR)
			continue;
		if (nr < 0) {
			perror("read() failed");
			exit(1);
		}
		if (nr == 0) {
			fprintf(stderr, "Reached read EOF!? Should never happen.\n");
			exit(0);
		}
		donebytes += nr;
	}

}
