
#ifndef __SCRAMBLER_H_
#define __SCRAMBLER_H_

#include <stdio.h>
#include <stdlib.h>

#include "lte_phy.h"

void GenScrambInt(int pScrambInt[], int n);
void Scrambling(int pInpSeq[], int pOutSeq[], int n);

void Descrambling(float pInpSeq[], float pOutBuf[], int n);

#endif
