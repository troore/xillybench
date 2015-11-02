#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // rand(), srand()
#include <time.h> // time()
#include <pthread.h>
#include <errno.h>

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

void *thread_read(void *param)
{
	int fdr;
	int *out;
	int nbytes, donebytes, rc;

	out = (int *)param;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	if (fdr < 0) {
		perror("Failed to open Xillybus device file(s) to read");
		exit(1);
	}

	nbytes = sizeof(int) * N * N;
	donebytes = 0;
	while (donebytes < nbytes) {
		rc = read(fdr, (void *)out + donebytes, nbytes - donebytes);
		printf("%d\n", rc);
		if ((rc < 0) && errno == EINTR)
			continue;
		if (rc < 0) {
			perror("read() failed");
			exit(1);
		}
		if (rc == 0) {
			fprintf(stderr, "Reached read EOF!? Should never happen.\n");
			exit(0);
		}
		donebytes += rc;
	}
	close(fdr);

	return NULL;
}

void *thread_write(void *param)
{
	int fdw;
	int *in;
	int nbytes, donebytes, rc;

	in = (int *)param;

	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if (fdw < 0) {
		perror("Failed to open Xillybus device file(s) to write");
		exit(1);
	}

	nbytes = sizeof(int) * 2 * N * N;
	donebytes = 0;
	while (donebytes < nbytes) {
		rc = write(fdw, (void *)in + donebytes, nbytes - donebytes);
		if ((rc < 0) && errno == EINTR)
			continue;
		if (rc <= 0) {
			perror("write() failed");
			exit(1);
		}
		donebytes += rc;
	}
	close(fdw);

	return NULL;
}

int main(int argc, char *argv[]) {

//	int fdr, fdw;
//	int nr, nw, donebytes;

	int *in, *out;
	int *golden_out;

	pthread_t tidr, tidw;

	double start, stop, elapsed_time;

//	fdr = open("/dev/xillybus_read_32", O_RDONLY);
//	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	/*
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	*/

	in = (int *)malloc((2 * N * N) * sizeof(int));
	out = (int *)malloc(N * N * sizeof(int));

	golden_out = (int *)malloc(N * N * sizeof(int));

	gen_mm_inputs(in, in + N * N, N);

	start = dtime();

//	nw = write(fdw, (void *)in, (2 * N * N) * sizeof(int));
//	nr = read(fdr, (void *)out, (N * N) * sizeof(int));
	pthread_create(&tidw, NULL, thread_write, out);
	pthread_create(&tidr, NULL, thread_read, in);

	pthread_join(tidw, NULL);
	pthread_join(tidr, NULL);

	stop = dtime();
	elapsed_time = stop - start;
	printf("Write time: %.3lfms\n", elapsed_time);

	golden_wrapper(in, golden_out);

	printf("%d\n", check_results(out, golden_out, N));

	free(in);
	free(out);
	free(golden_out);

//	close(fdr);
//	close(fdw);

	return 0;
}
