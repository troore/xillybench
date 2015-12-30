#ifndef __TRANSFORM_PRECODER_H_
#define __TRANSFORM_PRECODER_H_

#include <iostream>
#include <math.h>

#include "lte_phy.h"

void TransformPrecoding(float pInpSeq[], float pDataMatrix[], int NumLayer, int MDFT);
void TransformDecoding(float pDataMatrix[], float pDecSeq[], int NumLayer, int MDFT);

#endif
