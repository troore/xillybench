#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#define N	(1 << 16)

// ms
double dtime()
{
	double tmseconds = 0.0;
	struct timeval mytime;

	gettimeofday(&mytime, (struct timezone*)0);
	tmseconds = (double)(mytime.tv_sec + mytime.tv_usec * 1.0e-6);

	return tmseconds * 1000.0;
}

int main(int argc, char *argv[]) {

	int fdr, fdw;

	int *in, *out;

	int i;

	double start, stop, elapsed_time;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	// Not checking return values of write() and read(). This must be done
	// in a real-life program to ensure reliability.

	in = (int *)malloc(sizeof(int));
	out = (int *)malloc(N * sizeof(int));

	*in = 512;

	start = dtime();
	write(fdw, (void *) in, sizeof(int));
	stop = dtime();
	elapsed_time = stop - start;
	printf("Write time: %.3lf\n", elapsed_time);

	start = dtime();
	read(fdr, (void *) out, N * sizeof(int));
	stop = dtime();
	elapsed_time = stop - start;
	printf("Read time: %.3lf\n", elapsed_time);
//	loopback_test(in, out);


	printf("FPGA said:\n");

	/*
	for (i = 0; i < N; i++)
	{
		printf("%d:", i);
		printf("%d\n", out[i]);
	}
	*/

	free(in);
	free(out);

	close(fdr);
	close(fdw);

	return 0;
}
