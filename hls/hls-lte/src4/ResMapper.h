
#ifndef __RESMAPPER_H_
#define __RESMAPPER_H_

#include "lte_phy.h"

void SubCarrierDemapping(/*LTE_PHY_PARAMS *lte_phy_params, */
	float pInpData[],
	float pOutData[],
	int dmrs_symb_pos[2],
	int NumRxAntenna,
	int SCLoc,
	int NIFFT,
	int MDFT);

#endif
