#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include <time.h> // for time(NULL);
#include "timer.h"

#define N	(1 << 7)

void host_mm(int *a, int *b, int *c)
{
	int i, j, k;

	for(i = 0; i < (N / 2); i++) {
		for(j = 0; j < N; j++) {
			int sum = 0;

			for(k = 0; k < N; k++) {
				sum += a[i * N + k] * b[k * N + j];
			}
			c[i * N + j] = sum;

		}
	}
}

void golden_mm(
      int a[(N / 2)][N],
      int b[N][N],
      int c[(N / 2)][N])
{
   int i, j, k;

   for(i = 0; i < (N / 2); i++) {
      for(j = 0; j < N; j++) {
         int sum = 0;

         for(k = 0; k < N; k++) {
            sum += a[i][k] * b[k][j];
         }
		 c[i][j] = sum;

      }
   }

#ifdef GOLDEN
   printf("C:\n");
   for(i = 0; i < N; i++)
      for(j = 0; j < N; j++)
	  {
		 printf("%d", c[i][j]);
		 printf((j < N - 1) ? " " : "\n");
	  }
#endif
}

void golden_wrapper(int *in, int *out)
{
	int (*pa)[N], (*pb)[N], (*pc)[N];

	pa = (int (*)[N])(in);
	pb = (int (*)[N])(in + (N / 2) * N);
	pc = (int (*)[N])(out);
	golden_mm(pa, pb, pc);
}

void gen_mm_inputs(int *a, int *b, int n)
{
	int i, j;

	srand(time(NULL));
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
		//	a[i * n + j] = rand() % 10 + 1;
		//	b[i * n + j] = rand() % 10 + 1;
			a[i * n + j] = 1;
			b[i * n + j] = 1;
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
				printf("%d: %d %d\n", err_no, m1[i * n + j], m2[i * n + j]);
			}
		}
	}

	return err_no;
}

int main(int argc, char *argv[]) {

#ifdef XILLY
	int fdr, fdw;
#endif

	int *in, *out;
	int *golden_out;

	double start, stop, elapsed_time;

#ifdef XILLY
	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
#endif

	// Not checking return values of write() and read(). This must be done
	// in a real-life program to ensure reliability.

	in = (int *)malloc((2 * N * N) * sizeof(int));
	out = (int *)malloc(N * N * sizeof(int));

	golden_out = (int *)malloc(N * N * sizeof(int));

	gen_mm_inputs(in, in + N * N, N);

	start = dtime();
#ifdef XILLY
	host_mm(in, in + N * N, out);
	write(fdw, (void *)(in + (N / 2) * N), ((N / 2) * N + N * N) * sizeof(int));
	read(fdr, (void *)(out + (N / 2) * N), ((N / 2) * N) * sizeof(int));
#endif
	// split A and C, and do multiplication, then merge C
	host_mm(in, in + N * N, golden_out);
	golden_wrapper(in + (N / 2) * N, golden_out + (N / 2) * N);

	stop = dtime();
	elapsed_time = stop - start;
	printf("Write time: %.3lfms\n", elapsed_time);

#ifdef XILLY
	printf("%d\n", check_results(out, golden_out, N));
#endif

	free(in);
	free(out);
	free(golden_out);

#ifdef XILLY
	close(fdr);
	close(fdw);
#endif

	return 0;
}
