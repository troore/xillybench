#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "rapl_power.h"

#include "dct.h"
#include "dct_wrapper.h"

int main(int argc, char *argv[])
{
	dct_data_t a[N], b[N];

	int i;
	FILE *fp;

	double tstart, tend, ttime;

	fp = fopen("../in.dat", "r");
	for (i = 0; i < N; i++) {
		int tmp;
		fscanf(fp, "%d", &tmp);
		a[i] = tmp;
	}
	fclose(fp);

	tstart = dtime();

	rapl_power_start();
	dct_wrapper(a, b);
	rapl_power_stop();

	tend = dtime();
	ttime = tend - tstart;

	printf("time_total: %.3f s\n", ttime / 1000.0);

#ifndef CO
	fp = fopen("../out_host.dat", "w");
#else
	fp = fopen("../out_co.dat", "w");
#endif
	for (i = 0; i < N; i++) {
		fprintf(fp, "%d\n", b[i]);
	}
	fclose(fp);

	return 0;
}
