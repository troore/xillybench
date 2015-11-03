#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct_2d.h"
#include "timer.h"
#include "rapl_power.h"

int main(int argc, char *argv[])
{
	int fdr, fdw;

	dct_data_t *a, *b;

	FILE *fp;
	int i, j;

//	double start, finish, elapsed_time;

//	a = (dct_data_t *)malloc((len + 1) * sizeof(dct_data_t));
	a = (dct_data_t *)malloc(N * sizeof(dct_data_t));
	b = (dct_data_t *)malloc(N * sizeof(dct_data_t));

//	a[0] = sfn;
	fp = fopen("../in.dat", "r");
	for (i = 0; i < N; i++) {
		int tmp;
		fscanf(fp, "%d", &tmp);
		a[i] = tmp;
	}
	fclose(fp);

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

//	start = dtime();
	rapl_power_start();
	for (i = 0; i < FF; i++) {
	//	write(fdw, (void *)a, (len + 1) * sizeof(dct_data_t));
		write(fdw, (void *)a, N * sizeof(dct_data_t));
		read(fdr, (void *)b, N * sizeof(dct_data_t));
	}
	rapl_power_stop();
//	finish = dtime();
//	elapsed_time = finish - start;

//	printf("%.3fms\n", elapsed_time);

	free(a);
	free(b);

	close(fdr);
	close(fdw);

	return 0;
}
