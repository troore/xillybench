#include "TransformPrecoder.h"
#include "dft.h"
#include <cstring>

#define NUM_UL_SYMB_SF 14
#define NUM_LAYER 2
#define NUM_MDFT LTE_PHY_DFT_SIZE_30_72MHZ

void TransformDecoding(float pDataMatrix[N_TRANS_DECODER_IN_MAX], float pDecSeq[N_TRANS_DECODER_OUT_MAX], int NumLayer, int MDFT)
{
//	int NumLayer = lte_phy_params->N_rx_ant;
//	int MDFT = lte_phy_params->N_dft_sz;
#pragma HLS ARRAY_PARTITION variable=pDecSeq cyclic factor=2 dim=1
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	for(int nlayer=0;nlayer</*NumLayer*/NUM_LAYER;nlayer++)
	{
		for(int nsym=0;nsym</*NumULSymbSF*/NUM_UL_SYMB_SF-2;nsym++)
		{
#pragma HLS LOOP_FLATTEN
			int idx = nlayer*(MDFT*(NumULSymbSF-2))+nsym*MDFT;
			float norm = (float)sqrt((float)MDFT);
			dft(MDFT, pDataMatrix + idx * 2, pDecSeq + idx * 2, -1);
			for(int m = 0; m < /*MDFT*/ NUM_MDFT; m++)
			{
#pragma HLS PIPELINE II=4
#pragma HLS DEPENDENCE array inter false
				pDecSeq[2 * (idx + m) + 0] = pDecSeq[2 * (idx + m) + 0] / norm;
				pDecSeq[2 * (idx + m) + 1] = pDecSeq[2 * (idx + m) + 1] / norm;
			}                              
		}
	}
}
