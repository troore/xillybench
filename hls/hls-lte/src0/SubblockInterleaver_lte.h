
#ifndef __SUBBLOCKINTERLEAVER_LTE_H_
#define __SUBBLOCKINTERLEAVER_LTE_H_

#include <stdlib.h>
#include <cmath>
#include <iostream>

#include "lte_phy.h"

void SubblockInterleaving(int SeqLen, int pInpMtr[], int pOutMtr[]);

void SubblockDeInterleaving(int SeqLen, float pInpMtr[], float pOutMtr[]);

#endif

