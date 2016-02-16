#include "mm.h"

static void mm(int *a, int *b, int *c)
{
	int i, j, k;

	int h;
	for (h = 0; h < 1000; h++) {
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			int sum = 0;

			for(k = 0; k < N; k++) {
				sum += a[i * N + k] * b[k * N + j];
			}
			c[i * N + j] = sum;
		}
	}
	}
}

void mm_wrapper(int *in, int *out)
{
	int *a, *b, *c;

	a = in;
	b = in + N * N;
	c = out;

	mm(a, b, c);
}
