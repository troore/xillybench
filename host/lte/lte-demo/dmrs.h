
#ifndef __DMRS_H_
#define __DMRS_H_

#include <math.h>

#define PI 3.14159265358979

void geneDMRS(float *pDMRS, int N_layer, int N_dft);
void geneDMRS(float *pDMRSReal, float *pDMRSImag, int N_layer, int N_dft);

#endif
