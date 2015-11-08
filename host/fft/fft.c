#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include <math.h>
#include <string.h>

#define N 512
#define PI	3.14159265358979323846264338327950288

int check_vector(float *v1, float *v2, int n)
{
	int i;
	int err_n = 0;

	for (i = 0; i < n; i++)
	{
		if (fabs(v1[i] - v2[i]) > 0.01)
		{
			printf("%d\n", i);
			err_n++;
		}
	}

	return err_n;
}

void fft_nrvs_two_arrays(int n, float *a_real, float *a_imag,
			  float *y_real, float *y_imag,
			  int direction)
{
	int p, i, k;
	int lgn;
	float omega_m[2], omega[2];
	float t[2], u[2];
	float ang;

//	lgn = log2(n);
//	for (i = 0; i < n; i++)
//		y[i] = a[i];

	for (p = 1; p <= (n / 2); p <<= 1)
	{
		//	omega_m[0] = cos((2 * PI) / m);
		//	omega_m[1] = ((float)direction) * sin((2 * PI) / m);
		for (i = 0; i < (n >> 1); i++)
		{
			int o_idx = i + (n >> 1);
			
			k = i & (p - 1); // i % p
			ang = ((2 * PI * k) / (2 * p));
			omega[0] = cos(ang);
			omega[1] = ((float)direction) * sin(ang);
			// t = omega * a[i + n / 2];
			t[0] = omega[0] * a_real[o_idx] - omega[1] * a_imag[o_idx];
			t[1] = omega[0] * a_imag[o_idx] + omega[1] * a_real[o_idx];
			// u = a[i];
			u[0] = a_real[i];
			u[1] = a_imag[i];

			//	y[2 * i - k] = u + t;
			y_real[2 * i - k] = u[0] + t[0];
			y_imag[2 * i - k] = u[1] + t[1];
			//	y[2 * i - k + p] = u - t;
			y_real[2 * i - k + p] = u[0] - t[0];
			y_imag[2 * i - k + p] = u[1] - t[1];
		}
		
		for (i = 0; i < n; i++)
		{
			a_real[i] = y_real[i];
			a_imag[i] = y_imag[i];
		}
	}
}


int main(int argc, char *argv[]) 
{

	int fdr, fdw;
	void *in, *out;

	int n, k, err_real_n, err_imag_n;
	float *vin_real, *vin_imag;
	float *vout_real, *vout_imag;
	float *vtmp_real, *vtmp_imag;

	if (argc < 2)
	{
		printf("Usage: ./fft n\n");
		exit(1);
	}


	n = atoi(argv[1]);
	if (n > N)
	{
		printf("Number of FFT points cannot exceed %d.\n", N);
		exit(1);
	}

	/*
	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);

	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	*/

	vin_real = (float *)malloc(n * sizeof(float));
	vin_imag = (float *)malloc(n * sizeof(float));
	vout_real = (float *)malloc(n * sizeof(float));
	vout_imag = (float *)malloc(n * sizeof(float));
	vtmp_real = (float *)malloc(n * sizeof(float));
	vtmp_imag = (float *)malloc(n * sizeof(float));

	in = (void *)malloc(2 * sizeof(int) + 2 * n * sizeof(float));
	out = (void *)malloc(2 * n * sizeof(float));

	// Input
	for (k = 0; k < n; k++)
	{
		vin_real[k] = 0.125 * cos((2 * PI * k) / (float)n);
		vin_imag[k] = 0.125 * sin((2 * PI * k ) / (float)n);
		vtmp_real[k] = vin_real[k];
		vtmp_imag[k] = vin_imag[k];
	}

	// Begin FPGA calculation
	*((int *)in) = n;
	*((int *)in + 1) = -1;
	memcpy((float *)in + 2, vin_real, n * sizeof(float));
	memcpy((float *)in + n + 2, vin_imag, n * sizeof(float));

//	write(fdw, (void *)(&in), sizeof(in));
//	read(fdr, (void *)(&out), sizeof(out));

	fft_nrvs_two_arrays(*((int *)in), (float *)in + 2, (float *)in + n + 2, (float *)out, (float *)out + n, *((int *)in + 1));

	memcpy(vout_real, (float *)out, n * sizeof(float));
	memcpy(vout_imag, (float *)out + n, n * sizeof(float));
	// End FPGA calculation


	// Check result
	for (k = 0; k < n; k++)
	{
		vout_real[k] /= n;
		vout_imag[k] /= n;
	}

	fft_nrvs_two_arrays(n, vout_real, vout_imag, vtmp_real, vtmp_imag, 1);

	err_real_n = check_vector(vtmp_real, vin_real, n);
	err_imag_n = check_vector(vtmp_imag, vin_imag, n);
	printf("%d\n%d\n", err_real_n, err_imag_n);

	free(in);
	free(out);

	free(vin_real);
	free(vin_imag);
	free(vtmp_real);
	free(vtmp_imag);
	free(vout_real);
	free(vout_imag);

	/*
	close(fdr);
	close(fdw);
	*/

	return 0;
}
