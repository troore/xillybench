
#include <stdio.h>
#include <math.h>

#include "lte_phy.h"
#include "fft.h"

#define NUM_FFT LTE_PHY_FFT_SIZE_30_72MHZ

static inline int pow2(int n)
{
	return 1<<n;
}

static inline int log2(int n)
{
	int i = 0;

	while (n > 1)
	{
		n /= 2;
		i++;
	}
	return i;
}


void fft_nrvs_same_array_cyclic(int n, float *a, float *y, int direction)
{
	int p, i, k;
	int lgn;
	float omega_m[2], omega[2];
	float t[2], u[2];
	float ang;

//	lgn = log2(n);
//	for (i = 0; i < n; i++)
//		y[i] = a[i];

//	printf("FFT: %d\n", n);

	for (p = 1; p <= (/*n*/NUM_FFT / 2); p <<= 1)
	{
		//	omega_m[0] = cos((2 * PI) / m);
		//	omega_m[1] = ((float)direction) * sin((2 * PI) / m);
		for (i = 0; i < (/*n*/NUM_FFT >> 1); i++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			int o_idx = i + (n >> 1);
			
			k = i & (p - 1); // i % p
			ang = ((2 * PI * k) / (2 * p));
			omega[0] = cos(ang);
			omega[1] = ((float)direction) * sin(ang);
			// t = omega * a[i + n / 2];
			t[0] = omega[0] * a[2 * o_idx + 0] - omega[1] * a[2 * o_idx + 1];
			t[1] = omega[0] * a[2 * o_idx + 1] + omega[1] * a[2 * o_idx + 0];
			// u = a[i];
			u[0] = a[2 * i + 0];
			u[1] = a[2 * i + 1];

			//	y[2 * i - k] = u + t;
			y[2 * (2 * i - k) + 0] = u[0] + t[0];
			y[2 * (2 * i - k) + 1] = u[1] + t[1];
			//	y[2 * i - k + p] = u - t;
			y[2 * (2 * i - k + p) + 0] = u[0] - t[0];
			y[2 * (2 * i - k + p) + 1] = u[1] - t[1];
		}
		
		for (i = 0; i < /*n*/NUM_FFT; i++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			a[2 * i + 0] = y[2 * i + 0];
			a[2 * i + 1] = y[2 * i + 1];
		}
	}
}

