
#include <stdlib.h>

#include "ResMapper.h"

//#include "refs/dmrs.h"
#include "lte_phy.h"

//#include <iostream>

void SubCarrierMapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData)
{
	int NumLayer = lte_phy_params->N_tx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	int SCLoc = lte_phy_params->sc_loc;
	int NIFFT = lte_phy_params->N_fft_sz;
	int MDFT = lte_phy_params->N_dft_sz;

	int in_buf_sz = lte_phy_params->resm_in_buf_sz;
	int out_buf_sz = lte_phy_params->resm_out_buf_sz;

//	float DMRS[2 * (2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX)];
//	float *pDMRS = (float *)malloc(2 * (2 * NumLayer * MDFT) * sizeof(float));

//	geneDMRS(pDMRS, NumLayer, MDFT);
	float *pDMRS = lte_phy_params->DMRS;

	/*
	for (int nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		for (int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdx = nlayer * NumULSymbSF + nsym;

			for (int n = 0; n < NIFFT; n++)
			{
				pOutData[SymIdx * NIFFT + n] = std::complex<float>(1.0, 0.0);
			}
		}
	}
	*/
	int nsig = NumLayer * NumULSymbSF * NIFFT;

	for (int k = 0; k < nsig; k++)
	{
		pOutData[k] = 1.0;
		pOutData[out_buf_sz + k] = 0.0;
	}

	for (int nlayer = 0; nlayer < NumLayer; nlayer++)
	{
		int DMRSslot = 0;
//#pragma omp parallel for		
		for (int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdx = nlayer * NumULSymbSF + nsym;
				
			if (nsym == lte_phy_params->dmrs_symb_pos[DMRSslot])
			{
				for (int n = SCLoc; n < SCLoc + MDFT; n++)
				{
					//	pOutData[SymIdx * NIFFT + n] = DMRS[(DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)];
					pOutData[SymIdx * NIFFT + n] = pDMRS[(DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)];
					pOutData[out_buf_sz + SymIdx * NIFFT + n] = pDMRS[(2 * NumLayer * MDFT) + (DMRSslot * NumLayer + nlayer) * MDFT + (n - SCLoc)];
				}
				DMRSslot++;
			}
			else
			{
				for (int n = SCLoc; n < SCLoc + MDFT; n++)
				{
					//	pOutData[SymIdx * NIFFT + n] = pInpData[nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc];
					pOutData[SymIdx * NIFFT + n] = pInpData[nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc];
					pOutData[out_buf_sz + SymIdx * NIFFT + n] = pInpData[in_buf_sz + nlayer * (NumULSymbSF - 2) * MDFT + (nsym - DMRSslot) * MDFT + n - SCLoc];
					
				}
			}
		}
	}

//	free(pDMRS);
}

void SubCarrierDemapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData)
{
	int SCLoc = lte_phy_params->sc_loc;
	int NumRxAntenna = lte_phy_params->N_rx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	int NIFFT = lte_phy_params->N_fft_sz;
	int MDFT = lte_phy_params->N_dft_sz;

	int in_buf_sz = lte_phy_params->resdm_in_buf_sz;
	int out_buf_sz = lte_phy_params->resdm_out_buf_sz;

	//////////// Get DMRS /////////////
	for (int nrs = 0; nrs < 2; nrs++)
	{
		int DMRSPos = lte_phy_params->dmrs_symb_pos[nrs];
			
		for (int nrx = 0; nrx < NumRxAntenna; nrx++)
		{

			int SymIdxIn = nrx * NumULSymbSF + DMRSPos;
			int SymIdxOut = nrx * 2 + nrs;

			for (int n = 0 ; n < MDFT; n++)
			{
				pOutData[SymIdxOut * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
				pOutData[out_buf_sz + SymIdxOut * MDFT + n] = pInpData[in_buf_sz + SymIdxIn * NIFFT + (n + SCLoc)];
			}
		}
	}
		
	//////////// Get Data /////////////
	for(int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		int SymOutIdx = 0;
		
		for (int nsym = 0; nsym < NumULSymbSF; nsym++)
		{
			int SymIdxIn = NumULSymbSF * nrx + nsym;
			
			if(((nsym == lte_phy_params->dmrs_symb_pos[0]) || (nsym == lte_phy_params->dmrs_symb_pos[1])))
			{}
			else
			{
				int SymOutT = (NumULSymbSF - 2) * nrx + SymOutIdx + NumRxAntenna * 2;

				for (int n = 0; n < MDFT; n++)
				{
					pOutData[SymOutT * MDFT + n] = pInpData[SymIdxIn * NIFFT + (n + SCLoc)];
					pOutData[out_buf_sz + SymOutT * MDFT + n] = pInpData[in_buf_sz + SymIdxIn * NIFFT + (n + SCLoc)];
				}
				SymOutIdx++;
			}
		}
	}
}
