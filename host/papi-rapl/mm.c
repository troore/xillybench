
#include <stdio.h>
#include "rapl_power.h"

#define MATRIX_SIZE 1024

static double a[MATRIX_SIZE][MATRIX_SIZE];
static double b[MATRIX_SIZE][MATRIX_SIZE];
static double c[MATRIX_SIZE][MATRIX_SIZE];

/* Naive matrix multiply */
void run_test(int quiet) {

	double s;
	int i,j,k;

	if (!quiet) {
		printf("Doing a naive %dx%d MMM...\n",MATRIX_SIZE,MATRIX_SIZE);
	}

	for(i=0;i<MATRIX_SIZE;i++) {
		for(j=0;j<MATRIX_SIZE;j++) {
			a[i][j]=(double)i*(double)j;
			b[i][j]=(double)i/(double)(j+5);
		}
	}

	for(j=0;j<MATRIX_SIZE;j++) {
		for(i=0;i<MATRIX_SIZE;i++) {
			s=0;
			for(k=0;k<MATRIX_SIZE;k++) {
				s+=a[i][k]*b[k][j];
			}
			c[i][j] = s;
		}
	}

	s=0.0;
	for(i=0;i<MATRIX_SIZE;i++) {
		for(j=0;j<MATRIX_SIZE;j++) {
			s+=c[i][j];
		}
	}

	if (!quiet) printf("Matrix multiply sum: s=%lf\n",s);
}


int main(int argc, char **argv)
{
	rapl_power_start();
	
	run_test(0);

	rapl_power_stop();
	
	return 0;
}
