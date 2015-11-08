#include <stdio.h>
#include <stdlib.h>

#include "dct.h"
#include "timer.h"
#include "rapl_power.h"

int main() 
{
	dct_data_t a[FF * N], b[FF * N];
	int retval = 0, i, j;
	FILE *fp;

	double start, finish, elapsed_time;

	for (i = 0; i < FF; i++) {
		fp = fopen("../in.dat", "r");
		for (j = 0; j < N; j++) {
			int tmp;
			fscanf(fp, "%d", &tmp);
			a[i * N + j] = tmp;
		}
		fclose(fp);
	}

	start = dtime();
//	rapl_power_start();
	dct(a, b);
//	rapl_power_stop();
	finish = dtime();
	elapsed_time = finish - start;

	printf("%.3fms\n", elapsed_time);

	return retval;
}
