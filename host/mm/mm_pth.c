
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
