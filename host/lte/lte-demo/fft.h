
#ifndef __FFT_H_
#define __FFT_H_

void fft(int n, float (*a)[2], float (*y)[2], int direction);

void fft_recur(int n, float (*a)[2], float (*y)[2], int direction);

void fft_iter(int n, float a[], float y[], int direction);
void fft_iter(int n, float *a_real, float *a_imag, float *y_real, float *y_imag, int direction);

void fft_nrvs(int n, float *a, float *y, int direction);

void fft_nrvs(int n, float *a_real, float *a_imag, float *y_real, float *y_imag, int direction);

#endif
