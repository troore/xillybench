#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct_co.h"
#include "timer.h"

int main(int argc, char *argv[])
{
	int fdr, fdw;

	dct_data_t *a, *b;

	FILE *fp;
	int i, j;
	int len, sfn, ff;

	double start, finish, elapsed_time;

	sfn = atoi(argv[1]);

	len = sfn * N;

//	a = (dct_data_t *)malloc((len + 1) * sizeof(dct_data_t));
	a = (dct_data_t *)malloc(len * sizeof(dct_data_t));
	b = (dct_data_t *)malloc(len * sizeof(dct_data_t));

//	a[0] = sfn;
	for (i = 0; i < sfn; i++) {
		fp = fopen("../in.dat", "r");
		for (j = 0; j < N; j++) {
			int tmp;
			fscanf(fp, "%d", &tmp);
		//	a[1 + i * N + j] = tmp;
			a[i * N + j] = tmp;
		}
		fclose(fp);
	}

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	ff = DW / sfn;

	printf("%d\n", ff);

	start = dtime();
	for (i = 0; i < ff; i++) {
	//	write(fdw, (void *)a, (len + 1) * sizeof(dct_data_t));
		write(fdw, (void *)a, len * sizeof(dct_data_t));
		read(fdr, (void *)b, len * sizeof(dct_data_t));
	}
	finish = dtime();
	elapsed_time = finish - start;

	printf("%.3fms\n", elapsed_time);

	free(a);
	free(b);

	close(fdr);
	close(fdw);

	return 0;
}
