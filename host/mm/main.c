#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <time.h> // time()
#include <pthread.h>

#include <stdint.h>

#include "mm.h"
#include "mm_host.h"

#include "timer.h"
#include "rapl_power.h"

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

void golden_mm_wrapper(int *in, int *out)
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

int check_results(int *m1, int *m2, int n)
{
	int i, j;
	int err_no = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (m1[i * n + j] != m2[i * n + j]) {
				printf("%d: %d %d\n", i * n + j, m1[i * n + j], m2[i * n + j]);
				err_no++;
			}
		}
	}

	return err_no;
}


int main(int argc, char *argv[]) {

	int *in, *out;
	int *golden_out;

	double start, stop, elapsed_time;

	in = (int *)malloc((2 * N * N) * sizeof(int));
	out = (int *)malloc(N * N * sizeof(int));

	golden_out = (int *)malloc(N * N * sizeof(int));

	gen_mm_inputs(in, in + N * N, N);

	start = dtime();

	rapl_power_start();
	mm_wrapper(in, out);
	rapl_power_stop();

	stop = dtime();

	elapsed_time = stop - start;
	printf("T1: %.3lf ms\n", elapsed_time);

	golden_mm_wrapper(in, golden_out);


	printf("%d\n", check_results(out, golden_out, N));

	free(in);
	free(out);
	free(golden_out);

	return 0;
}
