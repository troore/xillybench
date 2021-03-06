#include <stdio.h>
#include <stdlib.h>

#include "dct_co.h"
#include "timer.h"

int main() {
	dct_data_t a[DW * N], b[DW * N];
	int retval = 0, i, j;
	FILE *fp;

	double start, finish, elapsed_time;

	for (i = 0; i < DW; i++) {
		fp = fopen("in.dat", "r");
		for (j = 0; j < N; j++) {
			int tmp;
			fscanf(fp, "%d", &tmp);
			a[i * N + j] = tmp;
		}
		fclose(fp);
	}

	start = dtime();
	dct(a, b);
	finish = dtime();
	elapsed_time = finish - start;

	/*
	for (i = 0; i < DW; i++) {
		fp = fopen("out.dat", "w");
		for (j = 0; j < N; j++) {
			fprintf(fp, "%d \n", b[i * N + j]);
		}
		fclose(fp);

		// Compare the results file with the golden results
		retval = system("diff --brief -w out.dat out.golden.dat");
		if (retval != 0) {
			printf("Test#%d\tfailed  !!!\n", i); 
			retval = 1;
		} else {
			printf("Test#%d\tpassed !\n", i);
		}
	}
	*/

	printf("%.3fms\n", elapsed_time);

	return retval;
}
