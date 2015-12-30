
#ifndef __RATEMATCHER_H_
#define __RATEMATCHER_H_

#include <cmath>
#include <iostream>

#include "lte_phy.h"
#include "SubblockInterleaver_lte.h"

//SubblockInterleaver_lte<int,int> SbInterleaver;
//SubblockInterleaver_lte<int,float> SbDeInterleaver;

void TxRateMatching(int pcMatrix[], int pcSeq[], int in_buf_sz, int out_buf_sz);
void RxRateMatching(float pLLRin[], float pLLRout[], int pHD[], int out_buf_sz);

#endif
