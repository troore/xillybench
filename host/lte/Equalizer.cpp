
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lte_phy.h"
#include "refs/dmrs.h"

static void MatrixProd(int d1, int d2, int d3, float M1[], float M2[], float oM[])
{
	int r, c, i;
	int m1_len = d1 * d2;
	int m2_len = d2 * d3;
	int om_len = d1 * d3;

	for (r = 0; r < d1; r++)
	{
		for (c = 0; c < d3; c++)
		{
			float tmp[2] = {0.0, 0.0};
			
			for (i = 0; i < d2; i++)
			{
				//	tmp += M1[r * d2 + i] * M2[i * d3 + c];
				tmp[0] += (M1[r * d2 + i] * M2[i * d3 + c] - M1[r * d2 + i + m1_len] * M2[i * d3 + c + m2_len]);
				tmp[1] += (M1[r * d2 + i] * M2[i * d3 + c + m2_len] + M1[r * d2 + i + m1_len] * M2[i * d3 + c]);
			}
		
			oM[r * d3 + c] = tmp[0];
			oM[r * d3 + c + om_len] = tmp[1];
		}
	}
}

static void MatrixProd(int d1, int d2, int d3,
					   float M1Real[], float M1Imag[],
					   float M2Real[], float M2Imag[],
					   float oMReal[], float oMImag[])
{
	int r, c, i;
	
	for (r = 0; r < d1; r++)
	{
		for (c = 0; c < d3; c++)
		{
			float tmp_real = 0.0;
			float tmp_imag = 0.0;
			
			for (i = 0; i < d2; i++)
			{
				tmp_real += (M1Real[r * d2 + i] * M2Real[i * d3 + c] - M1Imag[r * d2 + i] * M2Imag[i * d3 + c]);
				tmp_imag += (M1Real[r * d2 + i] * M2Imag[i * d3 + c] + M1Imag[r * d2 + i] * M2Real[i * d3 + c]);
			}
		
			oMReal[r * d3 + c] = tmp_real;
			oMImag[r * d3 + c] = tmp_imag;
		}
	}
}

static void MatrixInv(int sz, float pM[], float pInvM[])
{
	int m_len = sz * sz;
	int x_len = sz * (2 * sz);
//	float pX[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];
	float *pX = (float *)malloc(2 * x_len * sizeof(float));


	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			//	pX[r * (2 * sz) + c] = pM[r * sz + c];
			pX[r * (2 * sz) + c] = pM[r * sz + c];
			pX[r * (2 * sz) + c + x_len] = pM[r * sz + c + m_len];
		}
		for (int c = sz; c < 2 * sz; c++)
		{
			if (c == (r + sz))
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)1.0;
				pX[r * (2 * sz) + c] = 1.0;
				pX[r * (2 * sz) + c + x_len] = 0.0;
			}
			else
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)0.0;
				pX[r * (2 * sz) + c] = 0.0;
				pX[r * (2 * sz) + c + x_len] = 0.0;
			}
		}
	}

//	float pCurRow[2  * LTE_PHY_N_ANT_MAX * 2];
	int ae_row_len = 2 * sz;
	float *pCurRow = (float *)malloc(2 * ae_row_len * sizeof(float));
	
	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			float A[2], B[2];
			//	pCurRow[c] = pX[r * (2 * sz) + c] / pX[r * (2 * sz) + r];
			A[0] = pX[r * (2 * sz) + c];
			A[1] = pX[r * (2 * sz) + c + x_len];
			B[0] = pX[r * (2 * sz) + r];
			B[1] = pX[r * (2 * sz) + r + x_len];
			pCurRow[c] = (A[0] * B[0] + A[1] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
			pCurRow[c + ae_row_len] = (A[1] * B[0] - A[0] * B[1]) / (B[0] * B[0] + B[1] * B[1]);
		}

		for (int c = 0; c < (2 * sz); c++)
		{
			//	pX[r * (2 * sz) + c] = pCurRow[c];
			pX[r * (2 * sz) + c] = pCurRow[c];
			pX[r * (2 * sz) + c + x_len] = pCurRow[c + ae_row_len];
		}

		for (int er = r + 1; er < sz; er++)
		{
			float curC[2] = {pX[er * (2 * sz) + r], pX[er * (2 * sz) + r + x_len]};

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX[er * (2 * sz) + c] -= (curC[0] * pCurRow[c] - curC[1] * pCurRow[c + ae_row_len]);
				pX[er * (2 * sz) + c + x_len] -= (curC[0] * pCurRow[c + ae_row_len] + curC[1] * pCurRow[c]);
			}  
		}
	}

	for (int r = sz - 1; r >= 0; r--)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			//	pCurRow[c] = pX[r * (2 * sz) + c];
			pCurRow[c] = pX[r * (2 * sz) + c];
			pCurRow[c + ae_row_len] = pX[r * (2 * sz) + c + x_len];
		}

		for (int er = r - 1; er >= 0; er--)
		{
			float curC[2] = {pX[er * (2 * sz) + r], pX[er * (2 * sz) + r + x_len]};

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pX[er * (2 * sz) + c] -= (curC[0] * pCurRow[c] - curC[1] * pCurRow[c + ae_row_len]);
				pX[er * (2 * sz) + c + x_len] -= (curC[0] * pCurRow[c + ae_row_len] + curC[1] * pCurRow[c]);
			}  
		}
	}

	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			int col = c + sz;

			//	pInvM[r * sz + c] = pX[r * (2 * sz) + col];
			pInvM[r * sz + c] = pX[r * (2 * sz) + col];
			pInvM[r * sz + c + m_len] = pX[r * (2 * sz) + col + x_len];
		}

	}

	free(pX);
	free(pCurRow);
}

static void MatrixInv(int sz,
					  float pMReal[], float pMImag[],
					  float pInvMReal[], float pInvMImag[])
{
//	int m_len = sz * sz;
	int x_len = sz * (2 * sz);
//	float pX[LTE_PHY_N_ANT_MAX * 2 * LTE_PHY_N_ANT_MAX * 2];
	float *pXReal = (float *)malloc(x_len * sizeof(float));
	float *pXImag = (float *)malloc(x_len * sizeof(float));


	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			//	pX[r * (2 * sz) + c] = pM[r * sz + c];
			pXReal[r * (2 * sz) + c] = pMReal[r * sz + c];
			pXImag[r * (2 * sz) + c] = pMImag[r * sz + c];
		}
		for (int c = sz; c < 2 * sz; c++)
		{
			if (c == (r + sz))
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)1.0;
				pXReal[r * (2 * sz) + c] = 1.0;
				pXImag[r * (2 * sz) + c] = 0.0;
			}
			else
			{
				//	pX[r * (2 * sz) + c] = (std::complex<float>)0.0;
				pXReal[r * (2 * sz) + c] = 0.0;
				pXImag[r * (2 * sz) + c] = 0.0;
			}
		}
	}

//	float pCurRow[2  * LTE_PHY_N_ANT_MAX * 2];
	int ae_row_len = 2 * sz;
	float *pCurRowReal = (float *)malloc(ae_row_len * sizeof(float));
	float *pCurRowImag = (float *)malloc(ae_row_len * sizeof(float));
	
	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			float AReal, AImag, BReal, BImag;
			//	pCurRow[c] = pX[r * (2 * sz) + c] / pX[r * (2 * sz) + r];
			AReal = pXReal[r * (2 * sz) + c];
			AImag = pXImag[r * (2 * sz) + c];
			BReal = pXReal[r * (2 * sz) + r];
			BImag = pXImag[r * (2 * sz) + r];
			pCurRowReal[c] = (AReal * BReal + AImag * BImag) / (BReal * BReal + BImag * BImag);
			pCurRowImag[c] = (AImag * BReal - AReal * BImag) / (BReal * BReal + BImag * BImag);
		}

		for (int c = 0; c < (2 * sz); c++)
		{
			//	pX[r * (2 * sz) + c] = pCurRow[c];
			pXReal[r * (2 * sz) + c] = pCurRowReal[c];
			pXImag[r * (2 * sz) + c] = pCurRowImag[c];
		}

		for (int er = r + 1; er < sz; er++)
		{
			float curCReal = pXReal[er * (2 * sz) + r];
			float curCImag = pXImag[er * (2 * sz) + r];

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pXReal[er * (2 * sz) + c] -= (curCReal * pCurRowReal[c] - curCImag * pCurRowImag[c]);
				pXImag[er * (2 * sz) + c] -= (curCReal * pCurRowImag[c] + curCImag * pCurRowReal[c]);
			}  
		}
	}

	for (int r = sz - 1; r >= 0; r--)
	{
		for (int c = 0; c < (2 * sz); c++)
		{
			//	pCurRow[c] = pX[r * (2 * sz) + c];
			pCurRowReal[c] = pXReal[r * (2 * sz) + c];
			pCurRowImag[c] = pXImag[r * (2 * sz) + c];
		}

		for (int er = r - 1; er >= 0; er--)
		{
			float curCReal = pXReal[er * (2 * sz) + r];
			float curCImag = pXImag[er * (2 * sz) + r];

			for (int c = 0; c < (2 * sz); c++)
			{
				//	pX[er * (2 * sz) + c] -= curC * pCurRow[c];
				pXReal[er * (2 * sz) + c] -= (curCReal * pCurRowReal[c] - curCImag * pCurRowImag[c]);
				pXImag[er * (2 * sz) + c] -= (curCReal * pCurRowImag[c] + curCImag * pCurRowReal[c]);
			}  
		}
	}

	for (int r = 0; r < sz; r++)
	{
		for (int c = 0; c < sz; c++)
		{
			int col = c + sz;

			//	pInvM[r * sz + c] = pX[r * (2 * sz) + col];
			pInvMReal[r * sz + c] = pXReal[r * (2 * sz) + col];
			pInvMImag[r * sz + c] = pXImag[r * (2 * sz) + col];
		}

	}

	free(pXReal);
	free(pXImag);
	free(pCurRowReal);
	free(pCurRowImag);
}

void FDLSEstimation(float *pXt,
				    float *pXtDagger,
 					float *pYt,
					float *pHTranspose,
					int NumLayer,
					int NumRxAntenna)
{
	float pXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixProd(NumLayer, 2, NumLayer, pXtDagger, pXt, pXDX);

	float pInvXDX[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixInv(NumLayer, pXDX, pInvXDX);

	float pXDY[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	MatrixProd(NumLayer, 2, NumRxAntenna, pXtDagger, pYt, pXDY);

	MatrixProd(NumLayer, NumLayer, NumRxAntenna, pInvXDX, pXDY, pHTranspose);
								
	/*
	for (int i = 0; i < 4; i++)
	{
		printf("%f\n", pHTranspose[i]);
		printf("%f\n", pHTranspose[i + 4]);
	}
	*/
}

void FDLSEstimation(float *pXtReal, float *pXtImag,
				    float *pXtDaggerReal, float *pXtDaggerImag,
 					float *pYtReal, float *pYtImag,
					float *pHTransposeReal, float *pHTransposeImag,
					int NumLayer,
					int NumRxAntenna)
{
	float pXDXReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pXDXImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixProd(NumLayer, 2, NumLayer, pXtDaggerReal, pXtDaggerImag, pXtReal, pXtImag, pXDXReal, pXDXImag);

	float pInvXDXReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pInvXDXImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixInv(NumLayer, pXDXReal, pXDXImag, pInvXDXReal, pInvXDXImag);
 
	float pXDYReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pXDYImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	MatrixProd(NumLayer, 2, NumRxAntenna, pXtDaggerReal, pXtDaggerImag, pYtReal, pYtImag, pXDYReal, pXDYImag);

	MatrixProd(NumLayer, NumLayer, NumRxAntenna, pInvXDXReal, pInvXDXImag, pXDYReal, pXDYImag, pHTransposeReal, pHTransposeImag);

	/*
	for (int i = 0; i < 4; i++)
	{
		printf("%f\n", pHTransposeReal[i]);
		printf("%f\n", pHTransposeImag[i]);
	}
	*/
}

void FDLSEqualization(float *pInpData, float *pHTranspose, int m, int NumLayer, int NumRxAntenna, int MDFTPerUser, int NumULSymbSF, float *pOutData)
{
	int inp_len = NumLayer * NumULSymbSF * MDFTPerUser;
	int out_len = NumLayer * (NumULSymbSF - 2) * MDFTPerUser;
//////////////////// Freq Domain Equalize received Data /////////////////
	int h_len = NumRxAntenna * NumLayer;
	int hdagger_len = NumLayer * NumRxAntenna;
	int ht_len = h_len;
	float pH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pHDagger[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];
	float pInvHDH[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX * 2];

	float pHDY[LTE_PHY_N_ANT_MAX * 2];

	for (int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		for (int layer = 0; layer < NumLayer; layer++)
		{
			//	pH[nrx * NumLayer + layer] = pHTranspose[layer * NumRxAntenna + nrx];
			pH[nrx * NumLayer + layer] = pHTranspose[layer * NumRxAntenna + nrx];
			pH[nrx * NumLayer + layer + h_len] = pHTranspose[layer * NumRxAntenna + nrx + ht_len];
			//	pHDagger[layer * NumRxAntenna + nrx] = conj(pHTranspose[layer * NumRxAntenna + nrx]);
			pHDagger[layer * NumRxAntenna + nrx] = pHTranspose[layer * NumRxAntenna + nrx];
			pHDagger[layer * NumRxAntenna + nrx + hdagger_len] = (-1.0) * pHTranspose[layer * NumRxAntenna + nrx + ht_len];
		}
	}

	MatrixProd(NumLayer, NumRxAntenna, NumLayer, pHDagger, pH, pHDH);

	MatrixInv(NumLayer, pHDH, pInvHDH);

	////////////////// Equalizing Data /////////////////
	for (int nSymb = 0; nSymb < NumULSymbSF - 2; nSymb++)
	{
		int y_len = NumRxAntenna;
		float pYData[LTE_PHY_N_ANT_MAX * 2];

		for (int nrx = 0; nrx < NumRxAntenna; nrx++)
		{
			int IDX = (NumULSymbSF - 2) * nrx + nSymb + 2 * NumRxAntenna;
			//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
			//	pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
			pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
			pYData[nrx + y_len] = pInpData[IDX * MDFTPerUser + m + inp_len];
		}
		MatrixProd(NumLayer, NumRxAntenna, 1, pHDagger, pYData, pHDY);

		int x_len = NumLayer;
		float pXData[LTE_PHY_N_ANT_MAX * 2];
		MatrixProd(NumLayer, NumLayer, 1, pInvHDH, pHDY, pXData);
		
		for (int layer = 0; layer < NumLayer; layer++)
		{
			int IDX = (NumULSymbSF - 2) * layer + nSymb;
			
			//	*(*(pOutData+IDX)+m)=*(pXData+layer);
			//	pOutData[IDX * MDFTPerUser + m] = pXData[layer];
			pOutData[IDX * MDFTPerUser + m] = pXData[layer];
			pOutData[IDX * MDFTPerUser + m + out_len] = pXData[layer + x_len];
		}
	}
}

void FDLSEqualization(float *pInpDataReal, float *pInpDataImag,
					  float *pHTransposeReal, float *pHTransposeImag,
					  int m,
					  int NumLayer,
					  int NumRxAntenna,
					  int MDFTPerUser,
					  int NumULSymbSF,
					  float *pOutDataReal, float *pOutDataImag)
{
//	int inp_len = NumLayer * NumULSymbSF * MDFTPerUser;
//	int out_len = NumLayer * (NumULSymbSF - 2) * MDFTPerUser;
//////////////////// Freq Domain Equalize received Data /////////////////
//	int h_len = NumRxAntenna * NumLayer;
//	int hdagger_len = NumLayer * NumRxAntenna;
//	int ht_len = h_len;
	float pHReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pHImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pHDaggerReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pHDaggerImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pHDHReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pHDHImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pInvHDHReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
	float pInvHDHImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

	float pHDYReal[LTE_PHY_N_ANT_MAX];
	float pHDYImag[LTE_PHY_N_ANT_MAX];

	for (int nrx = 0; nrx < NumRxAntenna; nrx++)
	{
		for (int layer = 0; layer < NumLayer; layer++)
		{
			//	pH[nrx * NumLayer + layer] = pHTranspose[layer * NumRxAntenna + nrx];
			pHReal[nrx * NumLayer + layer] = pHTransposeReal[layer * NumRxAntenna + nrx];
			pHImag[nrx * NumLayer + layer] = pHTransposeImag[layer * NumRxAntenna + nrx];
			//	pHDagger[layer * NumRxAntenna + nrx] = conj(pHTranspose[layer * NumRxAntenna + nrx]);
			pHDaggerReal[layer * NumRxAntenna + nrx] = pHTransposeReal[layer * NumRxAntenna + nrx];
			pHDaggerImag[layer * NumRxAntenna + nrx] = (-1.0) * pHTransposeImag[layer * NumRxAntenna + nrx];
		}
	}

	MatrixProd(NumLayer, NumRxAntenna, NumLayer, pHDaggerReal, pHDaggerImag, pHReal, pHImag, pHDHReal, pHDHImag);

	MatrixInv(NumLayer, pHDHReal, pHDHImag, pInvHDHReal, pInvHDHImag);

	////////////////// Equalizing Data /////////////////
	for (int nSymb = 0; nSymb < NumULSymbSF - 2; nSymb++)
	{
		//	int y_len = NumRxAntenna;
		float pYDataReal[LTE_PHY_N_ANT_MAX];
		float pYDataImag[LTE_PHY_N_ANT_MAX];

		for (int nrx = 0; nrx < NumRxAntenna; nrx++)
		{
			int IDX = (NumULSymbSF - 2) * nrx + nSymb + 2 * NumRxAntenna;
			//	*(pYData+nrx)=*(*(pInpData+IDX)+m);
			//	pYData[nrx] = pInpData[IDX * MDFTPerUser + m];
			pYDataReal[nrx] = pInpDataReal[IDX * MDFTPerUser + m];
			pYDataImag[nrx] = pInpDataImag[IDX * MDFTPerUser + m];
		}
		
		MatrixProd(NumLayer, NumRxAntenna, 1, pHDaggerReal, pHDaggerImag, pYDataReal, pYDataImag, pHDYReal, pHDYImag);

		//	int x_len = NumLayer;
		float pXDataReal[LTE_PHY_N_ANT_MAX];
		float pXDataImag[LTE_PHY_N_ANT_MAX];
		
		MatrixProd(NumLayer, NumLayer, 1, pInvHDHReal, pInvHDHImag, pHDYReal, pHDYImag, pXDataReal, pXDataImag);
		
		for (int layer = 0; layer < NumLayer; layer++)
		{
			int IDX = (NumULSymbSF - 2) * layer + nSymb;
			
			//	*(*(pOutData+IDX)+m)=*(pXData+layer);
			//	pOutData[IDX * MDFTPerUser + m] = pXData[layer];
			pOutDataReal[IDX * MDFTPerUser + m] = pXDataReal[layer];
			pOutDataImag[IDX * MDFTPerUser + m] = pXDataImag[layer];
		}
	}
}

void LSFreqDomain(float *pInpData, float *pOutData, int MDFT, int NumLayer, int NumRxAntenna, int NumULSymbSF, float *pDMRS)
{
	int inp_len = NumLayer * NumULSymbSF * MDFT;
	int out_len = NumLayer * (NumULSymbSF - 2) * MDFT;
	int dmrs_len = NumLayer * MDFT * 2;
	
//	float *pDMRS = (float *)malloc(2 * dmrs_len * sizeof(float));
	
	geneDMRS(pDMRS, NumLayer, MDFT);

	for (int m = 0; m < MDFT; m++)
	{
		int xt_len = 2 * NumLayer;
		float pXt[2 * (2 * LTE_PHY_N_ANT_MAX)];
		float pXtDagger[2 * (LTE_PHY_N_ANT_MAX * 2)];

		for (int slot = 0; slot < 2; slot++)
		{
			for (int layer = 0; layer < NumLayer; layer++)
			{
				//	pXt[slot * NumLayer + layer] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXt[slot * NumLayer + layer] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXt[slot * NumLayer + layer + xt_len] = pDMRS[(slot * NumLayer + layer) * MDFT + m + dmrs_len];
				//	pXtDagger[layer * 2 + slot] = conj(pDMRS[(slot * NumLayer + layer) * MDFT + m]);
				pXtDagger[layer * 2 + slot] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXtDagger[layer * 2 + slot + xt_len] = (-1.0) * pDMRS[(slot * NumLayer + layer) * MDFT + m + dmrs_len];
			}
		}

		int yt_len = NumRxAntenna * 2;
		float pYt[2 * (2 * LTE_PHY_N_ANT_MAX)];

		for (int slot = 0; slot < 2; slot++)                                      
		{
			for (int nrx = 0; nrx < NumRxAntenna; nrx++)
			{
				//	pYt[slot * NumRxAntenna + nrx] = pInpData[(nrx * 2 + slot) * MDFT + m];
				pYt[slot * NumRxAntenna + nrx] = pInpData[(nrx * 2 + slot) * MDFT + m];
				pYt[slot * NumRxAntenna + nrx + yt_len] = pInpData[(nrx * 2 + slot) * MDFT + m + inp_len];
			}
		}

		float pHTranspose[2 * (LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX)];

		FDLSEstimation(pXt, pXtDagger, pYt, pHTranspose, NumLayer, NumRxAntenna);

		FDLSEqualization(pInpData, pHTranspose, m, NumLayer, NumRxAntenna, MDFT, NumULSymbSF, pOutData);
	}

//	free(pDMRS);
}

void LSFreqDomain(float *pInpDataReal, float *pInpDataImag,
				  float *pOutDataReal, float *pOutDataImag,
				  int MDFT, int NumLayer, int NumRxAntenna, int NumULSymbSF,
				  float *pDMRSReal, float *pDMRSImag)
{
//	int inp_len = NumLayer * NumULSymbSF * MDFT;
//	int out_len = NumLayer * (NumULSymbSF - 2) * MDFT;
//	int dmrs_len = NumLayer * MDFT * 2;
	
//	float *pDMRSReal = (float *)malloc(dmrs_len * sizeof(float));
//	float *pDMRSImag = (float *)malloc(dmrs_len * sizeof(float));
	
//	geneDMRS(pDMRSReal, pDMRSImag, NumLayer, MDFT);

//	float *pDMRSReal = lte_phy_params->DMRSReal;
//	float *pDMRSImag = lte_phy_params->DMRSImag;

	for (int m = 0; m < MDFT; m++)
	{
		//	int xt_len = 2 * NumLayer;
		float pXtReal[2 * LTE_PHY_N_ANT_MAX];
		float pXtImag[2 * LTE_PHY_N_ANT_MAX];
		float pXtDaggerReal[LTE_PHY_N_ANT_MAX * 2];
		float pXtDaggerImag[LTE_PHY_N_ANT_MAX * 2];

		for (int slot = 0; slot < 2; slot++)
		{
			for (int layer = 0; layer < NumLayer; layer++)
			{
				//	pXt[slot * NumLayer + layer] = pDMRS[(slot * NumLayer + layer) * MDFT + m];
				pXtReal[slot * NumLayer + layer] = pDMRSReal[(slot * NumLayer + layer) * MDFT + m];
				pXtImag[slot * NumLayer + layer] = pDMRSImag[(slot * NumLayer + layer) * MDFT + m];
				//	pXtDagger[layer * 2 + slot] = conj(pDMRS[(slot * NumLayer + layer) * MDFT + m]);
				pXtDaggerReal[layer * 2 + slot] = pDMRSReal[(slot * NumLayer + layer) * MDFT + m];
				pXtDaggerImag[layer * 2 + slot] = (-1.0) * pDMRSImag[(slot * NumLayer + layer) * MDFT + m];
			}
		}


		//	int yt_len = NumRxAntenna * 2;
		float pYtReal[2 * LTE_PHY_N_ANT_MAX];
		float pYtImag[2 * LTE_PHY_N_ANT_MAX];

		for (int slot = 0; slot < 2; slot++)                                      
		{
			for (int nrx = 0; nrx < NumRxAntenna; nrx++)
			{
				//	pYt[slot * NumRxAntenna + nrx] = pInpData[(nrx * 2 + slot) * MDFT + m];
				pYtReal[slot * NumRxAntenna + nrx] = pInpDataReal[(nrx * 2 + slot) * MDFT + m];
				pYtImag[slot * NumRxAntenna + nrx] = pInpDataImag[(nrx * 2 + slot) * MDFT + m];
			}
		}


		float pHTransposeReal[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];
		float pHTransposeImag[LTE_PHY_N_ANT_MAX * LTE_PHY_N_ANT_MAX];

		FDLSEstimation(pXtReal, pXtImag, pXtDaggerReal, pXtDaggerImag, pYtReal, pYtImag, pHTransposeReal, pHTransposeImag, NumLayer, NumRxAntenna);

		FDLSEqualization(pInpDataReal, pInpDataImag, pHTransposeReal, pHTransposeImag, m, NumLayer, NumRxAntenna, MDFT, NumULSymbSF, pOutDataReal, pOutDataImag);
	}

//	free(pDMRSReal);
//	free(pDMRSImag);
}

void Equalizing(LTE_PHY_PARAMS *lte_phy_params, float *pInpData, float *pOutData)
{
	int MDFT = lte_phy_params->N_dft_sz;
	int NumLayer = lte_phy_params->N_tx_ant;
	int NumRxAntenna = lte_phy_params->N_rx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	
	LSFreqDomain(pInpData, pOutData, MDFT, NumLayer, NumRxAntenna, NumULSymbSF, lte_phy_params->DMRS);
}

void Equalizing(LTE_PHY_PARAMS *lte_phy_params,
				float *pInpDataReal, float *pInpDataImag,
				float *pOutDataReal, float *pOutDataImag)
{
	int MDFT = lte_phy_params->N_dft_sz;
	int NumLayer = lte_phy_params->N_tx_ant;
	int NumRxAntenna = lte_phy_params->N_rx_ant;
	int NumULSymbSF = LTE_PHY_N_SYMB_PER_SUBFR;
	
	LSFreqDomain(pInpDataReal, pInpDataImag, pOutDataReal, pOutDataImag,
				 MDFT, NumLayer, NumRxAntenna, NumULSymbSF,
				 lte_phy_params->DMRSReal, lte_phy_params->DMRSImag);
}
