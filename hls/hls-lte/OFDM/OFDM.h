#ifndef __OFDM_H_
#define __OFDM_H_

#include "lte_phy.h"

void ofdemodulating(float pInpData[2 * N_OFDEMOD_IN_MAX], float pOutData[2 * N_OFDEMOD_OUT_MAX],
		int NumRxAntenna, int NIFFT, int CPLen);

void ofdemodulating(float pInpData[2 * N_OFDEMOD_IN_MAX], float pOutData[2 * N_OFDEMOD_OUT_MAX]);

#endif
