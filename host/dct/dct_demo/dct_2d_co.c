#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "dct.h"
#include "timer.h"
#include "rapl_power.h"

void dct_2d_co(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	int fdr, fdw;
	int nw, nr, donebytes;

	dct_data_t *a, *b;

	a = (dct_data_t *)in_block;
	b = (dct_data_t *)out_block;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	nw = write(fdw, (void *)a, N * sizeof(dct_data_t));
//	printf("%d\n", nw);
//	printf("%d\n", nr);
	donebytes = 0;
	while (donebytes < N * sizeof(int)) {
		nr = read(fdr, (void *)b, N * sizeof(dct_data_t));
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

	close(fdr);
	close(fdw);
}
