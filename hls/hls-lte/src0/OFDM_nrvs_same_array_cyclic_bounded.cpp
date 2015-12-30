
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fft.h"
#include "lte_phy.h"

#define NUM_RX_ANTENNA 2
#define NUM_UL_SYMB_SF LTE_PHY_N_SYMB_PER_SUBFR
#define NUM_FFT LTE_PHY_FFT_SIZE_1_92MHZ
#define N_OFDEMOD_IN	(NUM_RX_ANTENNA * (NUM_FFT + LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ) * NUM_UL_SYMB_SF)
#define N_OFDEMOD_OUT	(NUM_RX_ANTENNA * NUM_FFT * NUM_UL_SYMB_SF)

void ofdemodulating(float pInpData[2 * /*N_OFDEMOD_IN_MAX*/ N_OFDEMOD_IN], float pOutData[2 * /*N_OFDEMOD_OUT_MAX*/ N_OFDEMOD_OUT])
{
	int NumRxAntenna = NUM_RX_ANTENNA;
	int NIFFT = NUM_FFT;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	int CPLen = 9;

//	int in_buf_sz = lte_phy_params->ofdemod_in_buf_sz;
//	int out_buf_sz = lte_phy_params->ofdemod_out_buf_sz;

	int nrx, nsym, i;

//	float p_samp_in_buf[2 * LTE_PHY_FFT_SIZE_MAX];
//	float p_samp_out_buf[2 * LTE_PHY_FFT_SIZE_MAX];

	float p_samp_in_buf[2 * NUM_FFT];
	float p_samp_out_buf[2 * NUM_FFT];

//	printf("OFDM: %d %d %d\n", NumRxAntenna, NumULSymbSF, NIFFT);
	
	for (nrx = 0; nrx < /*NumRxAntenna*/ NUM_RX_ANTENNA; nrx++)
	{
		for (nsym = 0; nsym < /*NumULSymbSF*/ NUM_UL_SYMB_SF; nsym++)
		{
			int symb_idx = nrx * NumULSymbSF + nsym;
			float norm = (float)sqrt((float)NIFFT) /*(float)1*/;

			for (i = 0; i < /*NIFFT*/NUM_FFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				p_samp_in_buf[2 * i + 0] = pInpData[2 * (symb_idx * (CPLen + NIFFT) + CPLen + i) + 0];
				p_samp_in_buf[2 * i + 1] = pInpData[2 * (symb_idx * (CPLen + NIFFT) + CPLen + i) + 1];
			}

		//	fft_iter(NIFFT, p_samp_in_buf, p_samp_out_buf, 1);
			fft_nrvs_same_array_cyclic(NIFFT, p_samp_in_buf, p_samp_out_buf, 1);

			for(i = 0; i < /*NIFFT*/ NUM_FFT; i++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				pOutData[2 * (symb_idx * NIFFT + i) + 0] = p_samp_out_buf[2 * i + 0] / norm;
				pOutData[2 * (symb_idx * NIFFT + i) + 1] = p_samp_out_buf[2 * i + 1] / norm;
			}
		}
	}
}

