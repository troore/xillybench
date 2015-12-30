
#include <stdio.h>
#include "dmrs.h"
#include "lte_phy.h"

#define NUM_RX_ANTENNA 2
#define NUM_DFT  LTE_PHY_DFT_SIZE_1_92MHZ
#define NUM_UL_SYMB_SF LTE_PHY_N_SYMB_PER_SUBFR
#define N_RESDEMAPPER_IN (NUM_RX_ANTENNA * LTE_PHY_FFT_SIZE_1_92MHZ * LTE_PHY_N_SYMB_PER_SUBFR)
#define N_RESDEMAPPER_OUT (NUM_RX_ANTENNA * LTE_PHY_DFT_SIZE_1_92MHZ * LTE_PHY_N_SYMB_PER_SUBFR)

void SubCarrierDemapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[/*N_RESDEMAPPER_IN_MAX*/ N_RESDEMAPPER_IN * 2],
	float pOutData[/*N_RESDEMAPPER_OUT_MAX*/ N_RESDEMAPPER_OUT * 2],
	int dmrs_symb_pos[2],
	int NumRxAntenna,
	int SCLoc,
	int NIFFT,
	int MDFT)
{
#pragma HLS DEPENDENCE variable=pOutData array intra WAW false
#pragma HLS ARRAY_PARTITION variable=pOutData block factor=2 dim=1
#pragma HLS ARRAY_PARTITION variable=pInpData block factor=2 dim=1
//	int SCLoc = lte_phy_params->sc_loc;
//	int NumRxAntenna = lte_phy_params->N_rx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
//	int NIFFT = lte_phy_params->N_fft_sz;
//	int MDFT = lte_phy_params->N_dft_sz;

//	printf("SubCarrierDemapping: %d %d %d\n", NumRxAntenna, NumULSymbSF, MDFT);
	//////////// Get DMRS /////////////
	for (int nrs = 0; nrs < 2; nrs++)
	{
		int DMRSPos = /*lte_phy_params->*/dmrs_symb_pos[nrs];
			
		for (int nrx = 0; nrx < /*NumRxAntenna*/NUM_RX_ANTENNA; nrx++)
		{
			int SymIdxIn = nrx * NumULSymbSF + DMRSPos;
			int SymIdxOut= nrx * 2 + nrs;

			for (int n = 0; n < /*MDFT*/ NUM_DFT; n++)
			{
#pragma HLS PIPELINE
				//	pOutData[SymIdxOut * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
				pOutData[2 * (SymIdxOut * MDFT + n) + 0] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 0];
				pOutData[2 * (SymIdxOut * MDFT + n) + 1] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 1];
			}
		}
	}
		
	//////////// Get Data /////////////
	for (int nrx = 0; nrx < /*NumRxAntenna*/ NUM_RX_ANTENNA; nrx++)
	{
		int SymOutIdx = 0;
		for(int nsym = 0; nsym < /*NumULSymbSF*/ NUM_UL_SYMB_SF; nsym++)
		{
			int SymIdxIn = NumULSymbSF * nrx + nsym;
			if(((nsym == /*lte_phy_params->*/dmrs_symb_pos[0]) || (nsym == /*lte_phy_params->*/dmrs_symb_pos[1])))
			{}
			else
			{
				int SymOutT = (NumULSymbSF - 2) * nrx + SymOutIdx+NumRxAntenna * 2;

				for (int n = 0; n < /*MDFT*/ NUM_DFT; n++)
				{
#pragma HLS PIPELINE
					//	pOutData[SymOutT * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
					pOutData[2 * (SymOutT * MDFT + n) + 0] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 0];
					pOutData[2 * (SymOutT * MDFT + n) + 1] = pInpData[2 * (SymIdxIn * NIFFT + (n + SCLoc)) + 1];
				}
				SymOutIdx++;
			}
		}
	}
}
