
#include <stdio.h>
#include <math.h>
#include "lte_phy.h"

#define N_LAYER 2
#define N_DFT 1200
#define NZC 1193

#define PI 3.14159265358979

void geneDMRS(float pDMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2],
			  int N_layer,
			  int N_dft)
{
#pragma HLS ARRAY_PARTITION variable=pDMRS cyclic factor=2 dim=1
	int i;
	
	int pPrimeTable[6][2];
	int ncs[2] = {3, 11};
	int Nzc;
	int zc_flag;
	int idx;
	int RSu, RSv;
	double qbar, q;
//	std::complex<double> px[1200];
	double px[1200][2];

	pPrimeTable[0][0] = 75;		pPrimeTable[0][1] = 73;
	pPrimeTable[1][0] = 150;	pPrimeTable[1][1] = 149;
	pPrimeTable[2][0] = 300;	pPrimeTable[2][1] = 293;
	pPrimeTable[3][0] = 600;	pPrimeTable[3][1] = 599;
	pPrimeTable[4][0] = 900;	pPrimeTable[4][1] = 887;
	pPrimeTable[5][0] = 1200;	pPrimeTable[5][1] = 1193;

	zc_flag = 1;
	idx = 0;
/*
	look_up_prime_table_loop:while (zc_flag)
	{
#pragma HLS PIPELINE
		if (pPrimeTable[idx][0] == N_dft)
		{ 
			Nzc = pPrimeTable[idx][1];
			zc_flag = 0;
		}
		else
		{
			idx++;
		}
	}
*/
	Nzc = NZC;

//	printf("geneDMRS: %d %d %d\n", N_layer, N_dft, Nzc);
	
	float tmp1;
	int tmp2;
	
	RSu = 0;
	RSv = 0;
	qbar = (double)Nzc * (RSu + 1.0) / 31.0;
	tmp1 = floor(2.0 * qbar);
	tmp2 = 1.0;
	for (i = 1; i <= (int)tmp1; i++)
		tmp2 = tmp2 * (-1.0);
	q = floor((qbar + 0.5)) + RSv * /*pow(-1.0,(floor((2.0*qbar))))*/ tmp2;


	double theta_pre = -PI * q / (double)Nzc;
	calculate_px_array_loop:for (int m = 0; m < /*Nzc*/ NZC; m++)
	{
#pragma HLS PIPELINE
		double theta = theta_pre *  m * (m + 1.0);
		//	px[m] = std::complex<double>(cos(theta), sin(theta));
		px[m][0] = cos(theta);
		px[m][1] = sin(theta);
	}

	calculate_pDMRS_from_px_loop:for (int slot = 0; slot < 2; slot++)
	{
		for (int layer = 0; layer < /*N_layer*/ N_LAYER; layer++)
		{
#pragma HLS LOOP_FLATTEN
			int cs = ncs[slot] + 6 * layer;
			double alpha = 2.0 * PI * cs / 12.0;
			for (int n = 0; n < /*N_dft*/N_DFT; n++)
			{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
				int index = ((slot * N_layer + layer) * N_dft + n) * 2 ;
				int idx = n % Nzc;
				double a = cos(alpha * n);
				double b = sin(alpha * n);
				//	std::complex<double> c = std::complex<double>((cos(alpha*n)),(sin(alpha*n)));
				//	std::complex<double> c = std::complex<double>(a, b);
				double c[2] = {a, b};
				//	*(*(*(pDMRS+slot)+layer)+n)=(complex<float>)(c * px[idx]);
				//	pDMRS[(slot * N_layer + layer) * N_dft + n] = (std::complex<float>)(c * px[idx]);
				pDMRS[index] = c[0] * px[idx][0] - c[1] * px[idx][1];
				pDMRS[index + 1] = c[0] * px[idx][1] + c[1] * px[idx][0];
			}
		}
	}

	if (N_layer == 2)
	{
		int index_offset = 2 * (1 * N_layer + 1) * N_dft;
		negate_pDMRS_loop:for (int n = 0; n < /*N_dft*/N_DFT; n ++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			//	(*(*(*(pDMRS+1)+1)+n))*=(-1.0);
			//	pDMRS[(1 * N_layer + 1) * N_dft + n] *= (-1.0);
			pDMRS[index_offset + 2 * n] = -1.0 * pDMRS[index_offset + 2 * n];
			pDMRS[index_offset + 2 * n + 1] = -1.0 * pDMRS[index_offset + 2 * n + 1];
		}
	}
}

