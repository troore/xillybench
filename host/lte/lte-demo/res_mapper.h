
#ifndef __RESMAPPER_H_
#define __RESMAPPER_H_

#include "lte_phy.h"

void SubCarrierMapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutBuf);
void SubCarrierMapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpDataReal, float *pInpDataImag,
					   float *pOutDataReal, float *pOutDataImag);
void SubCarrierDemapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutBuf);
void SubCarrierDemapping(LTE_PHY_PARAMS *lte_phy_params, float *pInpDataReal, float *pInpDataImag,
					   float *pOutDataReal, float *pOutDataImag);

#endif
