
#ifndef __EQUALIZER_H_
#define __EQUALIZER_H_

#include "lte_phy.h"

void FDLSEstimation(float pXt[], float pXtDagger[], float pYt[], float pHTranspose[], int NumLayer, int NumRxAntenna);

void FDLSEqualization(float pInpData[], float pHTranspose[], int m, int NumLayer, int NumRxAntenna, int MDFTPerUser, float pOutData[]);

void LSFreqDomain(float pInpData[], float pOutData[], int MDFT, int NumLayer, int NumRxAntenna);

void Equalizing(float pInpData[], float pOutData[], int MDFT, int NumLayer, int NumRxAntenna);

#endif
