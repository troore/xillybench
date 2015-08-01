#include <math.h>
#include <stdint.h>
#include "xilly_debug.h"

#define PI	3.14159265358979323846264338327950288
#define N 512

//float a_real[N], a_imag[N], y_real[N], y_imag[N];

/*
   int mycalc(int a, float *x2) {
 *x2 = sinf(*x2);
 return a + 1;
 }
 */

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


void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int i;
	int n, direction;
//	float *a_real, *a_imag, *y_real, *y_imag;
	float a_real[N], a_imag[N], y_real[N], y_imag[N];

	xilly_puts("Hello, world\n");

	// Handle input data
	n = *in++;
	direction = *in++;

//	a_real = (float *)in;
//	a_imag = (float *)in + n;
//	y_real = (float *)out;
//	y_imag = (float *)out + n;

	for (i = 0; i < n; i++)
	{
		int tmp;
		tmp = in[i];
		//a_real[i] = *((float *)&(in[i]));
		a_real[i] = *((float *)&tmp);
	//	a_real[i] = ((float *)in)[i];
		tmp = in[i + n];
		a_imag[i] = *((float *)&tmp);
	//	a_imag[i] = ((float *)in)[i + n];
	}

	// Debug output
	// Add some debug outputs declared in xilly_debug.h

	// Run the calculations
	fft_nrvs_two_arrays(n, a_real, a_imag, y_real, y_imag, direction);

	// Handle output data
	for (i = 0; i < n; i++)
	{
		float tmp;
		tmp = y_real[i];
	//	out[i] = *((int *)&(y_real[i]));
		out[i] = *((int *)&tmp);
		tmp = y_imag[i];
	//	out[i + n] = *((int *)&(y_imag[i]));
		out[i + n] = *((int *)&tmp);
	}
}
