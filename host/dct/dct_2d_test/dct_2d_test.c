#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct_2d.h"
#include "timer.h"
#include "rapl_power.h"

int main(int argc, char *argv[])
{
	dct_data_t *a, *b;
	FILE *fp;

	int i;

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

//	start = dtime();
	rapl_power_start();
	for (i = 0; i < FF; i++) {
	//	write(fdw, (void *)a, (len + 1) * sizeof(dct_data_t));
		dct_2d_test(a, b);
	}
	rapl_power_stop();
//	finish = dtime();
//	elapsed_time = finish - start;

//	printf("%.3fms\n", elapsed_time);

	free(a);
	free(b);

	return 0;
}
