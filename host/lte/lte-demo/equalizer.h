

#ifndef __EQUALIZER_H_
#define __EQUALIZER_H_

#include "lte_phy.h"

void FDLSEstimation(float *pXt, float *pXtDagger, float *pYt, float *pHTranspose, int NumLayer, int NumRxAntenna);
void FDLSEstimation(float *pXtReal, float *pXtImag,
				    float *pXtDaggerReal, float *pXtDaggerImag,
 					float *pYtReal, float *pYtImag,
					float *pHTransposeReal, float *pHTransposeImag,
					int NumLayer,
					int NumRxAntenna);

void FDLSEqualization(float *pInpData, float *pHTranspose, int m, int NumLayer, int NumRxAntenna, int MDFTPerUser, int NumULSymbSF, float *pOutData);
void FDLSEqualization(float *pInpDataReal, float *pInpDataImag,
					  float *pHTransposeReal, float *pHTransposeImag,
					  int m,
					  int NumLayer,
					  int NumRxAntenna,
					  int MDFTPerUser,
					  int NumULSymbSF,
					  float *pOutDataReal, float *pOutDataImag);

void LSFreqDomain(float *pInpData, float *pOutData, int MDFT, int NumLayer, int NumRxAyntenna, int NumULSymbSF, float *pDMRS);
void LSFreqDomain(float *pInpDataReal, float *pInpDataImag,
				  float *pOutDataReal, float *pOutDataImag,
				  int MDFT, int NumLayer, int NumRxAntenna, int NumULSymbSF,
				  float *pDMRSReal, float *pDMRSImag);

void Equalizing(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData);
void Equalizing(LTE_PHY_PARAMS *lte_phy_params,
				float *pInpDataReal, float *pInpDataImag,
				float *pOutDataReal, float *pOutDataImag);

#endif
