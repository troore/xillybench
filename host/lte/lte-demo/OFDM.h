
#ifndef __OFDM_H_
#define __OFDM_H_

#include "lte_phy.h"

void ofmodulating(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData);
void ofmodulating(LTE_PHY_PARAMS *lte_phy_params, float *pInpDataReal, float *pInpDataImag,
				  float *pOutDataReal, float *pOutDataImag);
void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData);
void ofdemodulating(LTE_PHY_PARAMS *lte_phy_params, float *pInpDataReal, float *pInpDataImag,
					float *pOutDataReal, float *pOutDataImag);

#endif
