#include <stdio.h>
#include <stdlib.h>

#include "dct.h"
//#include "timer.h"

int main(int argc, char *argv[])
{
	short a[N], b[N];
	int i;
	FILE *fp;

	fp = fopen("../in.dat", "r");
	for (i = 0; i < N; i++) {
		int tmp;
		fscanf(fp, "%d", &tmp);
		a[i] = tmp;
	}
	fclose(fp);

	dct(a, b);

	return 0;
}
