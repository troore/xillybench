#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mm.h"

void mm_wrapper(int *in, int *out)
{
	int fdr, fdw;
	int nr, nw, donebytes;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	// Not checking return values of write() and read(). This must be done
	// in a real-life program to ensure reliability.
	nw = write(fdw, (void *)in, (2 * N * N) * sizeof(int));
	printf("%d\n", nw);
	donebytes = 0;
	while (donebytes < N * N * sizeof(int)) {
		nr = read(fdr, (void *)out + donebytes, (N * N) * sizeof(int) - donebytes);
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
