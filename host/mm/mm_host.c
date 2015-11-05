#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // rand(), srand()
#include <time.h> // time()

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include "timer.h"

#define N	(1 << 8)

void golden_mm(
      int *a,
      int *b,
      int *c)
{
   int i, j, k;

   for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
         int sum = 0;

         for(k = 0; k < N; k++) {
            sum += a[i * N + k] * b[k * N + j];
         }
		 c[i * N + j] = sum;

      }
   }

#ifdef GOLDEN
   printf("C:\n");
   for(i = 0; i < N; i++)
      for(j = 0; j < N; j++)
	  {
		 printf("%d", c[i * N + j]);
		 printf((j < N - 1) ? " " : "\n");
	  }
#endif
}

void golden_wrapper(int *in, int *out)
{
	int i;

	for (i = 0; i < /*10000*/1; i++) {
		golden_mm(in, in + N * N, out);
	}
}

void gen_mm_inputs(int *a, int *b, int n)
{
	int i, j;

	srand(time(NULL));
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			a[i * n + j] = rand() % 10 + 1;
			b[i * n + j] = rand() % 10 + 1;
		}

#ifdef GOLDEN
	printf("A:\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d", a[i * n + j]);
			printf(j < n - 1 ? " " : "\n");
		}
	}
	printf("B:\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d ", b[i * n + j]);
			printf(j < n - 1 ? " " : "\n");
		}
	}
#endif
}

int main(int argc, char *argv[]) {

	int *in, *out;

	double start, stop, elapsed_time;

	// Not checking return values of write() and read(). This must be done
	// in a real-life program to ensure reliability.

	in = (int *)malloc((2 * N * N) * sizeof(int));
	out = (int *)malloc(N * N * sizeof(int));

	gen_mm_inputs(in, in + N * N, N);

	start = dtime();

	golden_wrapper(in, out);

	stop = dtime();
	elapsed_time = stop - start;
	printf("Write time: %.3lfms\n", elapsed_time);

	free(in);
	free(out);

	return 0;
}
