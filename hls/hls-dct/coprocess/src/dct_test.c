// Copyright (C) 2008 AutoESL Design Technologies, Inc.
// All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include "dct.h"

// ********************************************************
int main() 
{
	short a[N], b[N];
	int i;
	FILE *fp;

	fp = fopen("in.dat", "r");
	for (i = 0; i < N; i++) {
		int tmp;
		fscanf(fp, "%d", &tmp);
		a[i] = tmp;
	}
	fclose(fp);

	dct_test(a, b);

	fp = fopen("out.dat", "w");
	//printf(" Din Dout \n");
	for (i = 0; i < N; i++) {
		fprintf(fp, "%d \n", b[i]);
		//printf("  %d   %d\n", a[i], b[i]);
	}
	fclose(fp);

	return 0;
}
