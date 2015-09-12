#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include <time.h>

#include "timer.h"

#define N	(1 << 3)

void golden_mm(
      int a[N][N],
      int b[N][N],
      int c[N][N])
{
   int i, j, k;

   for(i = 0; i < N; i++) {
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
	int n;
	int (*pa)[N], (*pb)[N], (*pc)[N];

	n = in[0];
	pa = (int (*)[N])(in + 1);
	pb = (int (*)[N])(in + 1 + n * n);
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
				printf("%d: %d %d\n", err_no, m1[i * n + j], m2[i * n + j]);
			}
		}
	}

	return err_no;
}


// ms
/*
double dtime()
{
	double tmseconds = 0.0;
	struct timeval mytime;

	gettimeofday(&mytime, (struct timezone*)0);
	tmseconds = (double)(mytime.tv_sec + mytime.tv_usec * 1.0e-6);

	return tmseconds * 1000.0;
}
*/

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

	in = (int *)malloc((1 + 2 * N * N) * sizeof(int));
	out = (int *)malloc(N * N * sizeof(int));

	golden_out = (int *)malloc(N * N * sizeof(int));

	in[0] = N;

	gen_mm_inputs(in + 1, in + 1 + N * N, N);

	start = dtime();
#ifdef XILLY
	write(fdw, (void *) in, (1 + 2 * N * N) * sizeof(int));
	read(fdr, (void *) out, (N * N) * sizeof(int));
#endif
	golden_wrapper(in, golden_out);

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
