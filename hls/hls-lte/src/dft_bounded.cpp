#include <math.h>
#include "dft.h"
/*
   Discrete Fourier Transform
 */
//void dft(int n, float a[N], float y[N], int direction)
void dft(int n, float a[2 * N], float y[2 * N], int direction)
{
	int k, j;
//	float p0[1201], p1[1201];
	float p0[N], p1[N];
	p0[0] = 0.0; p1[0] = 0.0;
	float ang;
	float pre_ang = (float)direction * TWO_PI / (float)n;
	dft_loop1:for (k = 0; k < /*n*/ N; k++)
	{
		dft_loop1_1:for (j = 0; j < /*n*/ N; j++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
#pragma HLS loop_flatten
            if(j == 0){
                ang = pre_ang * ((float)k);
            }
            if(j == /*n*/ N - 1){
            	y[2 * k + 0] = p0[j] + (a[2 * j] * cosf(j * ang) - a[2 * j + 1] * sinf(j * ang));
            	y[2 * k + 1] = p1[j] + (a[2 * j] * sinf(j * ang) - a[2 * j + 1] * cosf(j * ang));
            }
            else{
				p0[j + 1] = p0[j] + (a[2 * j] * cosf(j * ang) - a[2 * j + 1] * sinf(j * ang));
				p1[j + 1] = p1[j] + (a[2 * j] * sinf(j * ang) - a[2 * j + 1] * cosf(j * ang));
            }
		}
	}
}

