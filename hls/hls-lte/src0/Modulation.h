
#ifndef __MODULATION_H_
#define __MODULATION_H_

#include "lte_phy.h"

#include <math.h>

//typedef float (*p_a)[2];

//void set_mod_params(p_a *pp_table, int *bits_per_samp, int *mod_table_len, int mod_type);
void init_mod_tables();

//void Modulating(int pBitsSeq[N_MOD_IN_MAX], float pQAMSeq[N_MOD_OUT_MAX * 2], int in_buf_sz, int mod_type);

void Demodulating(float pDecQAMSeq[N_MOD_OUT_MAX * 2], float pLLR[N_MOD_IN_MAX], int in_buf_sz, int mod_type, float awgnSigma);

//void Demodulating(std::complex<float> pDecQAMSeq[N_MOD_OUT_MAX], int pHD[N_MOD_IN_MAX], int in_buf_sz, int mod_type);

#endif
