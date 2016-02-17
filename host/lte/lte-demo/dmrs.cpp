
#include <math.h>

#define PI 3.14159265358979

void geneDMRS(float *pDMRS, int N_layer, int N_dft)
{
	int i;
	int table_len = 2 * N_layer * N_dft;
	
	int pPrimeTable[6][2];
	int ncs[2] = {3, 11};
	int Nzc;
	int zc_flag;
	int idx;
	int RSu, RSv;
	double qbar, q;
//	std::complex<double> px[1200];
	double px[2][1200];
	
	pPrimeTable[0][0] = 75;		pPrimeTable[0][1] = 73;
	pPrimeTable[1][0] = 150;	pPrimeTable[1][1] = 149;
	pPrimeTable[2][0] = 300;	pPrimeTable[2][1] = 293;
	pPrimeTable[3][0] = 600;	pPrimeTable[3][1] = 599;
	pPrimeTable[4][0] = 900;	pPrimeTable[4][1] = 887;
	pPrimeTable[5][0] = 1200;	pPrimeTable[5][1] = 1193;

	zc_flag = 1;
	idx = 0;
	while (zc_flag)
	{
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

	RSu = 0;
	RSv = 0;
	qbar = ((double)Nzc) * (RSu + 1.0) / 31.0;
	q = floor((qbar + 0.5)) + RSv * pow(-1.0, (floor((2.0 * qbar))));

	for (int m = 0; m < Nzc; m++)
	{
		double theta = (-PI * q * m * (m + 1.0)) / ((double)Nzc);
		//	px[m] = std::complex<double>(cos(theta), sin(theta));
		px[0][m] = cos(theta);
		px[1][m] = sin(theta);
	}

	for (int slot = 0; slot < 2; slot++)
	{
		for (int layer = 0; layer < N_layer; layer++)
		{
			int cs = ncs[slot] + 6 * layer;
			double alpha = (2.0 * PI * cs) / 12.0;

			for (int n = 0; n < N_dft; n++)
			{
				int idx = (n % Nzc);
				double a = cos(alpha * n);
				double b = sin(alpha * n);
				//	std::complex<double> c = std::complex<double>((cos(alpha*n)),(sin(alpha*n)));
				double c[2] = {a, b};

				pDMRS[(slot * N_layer + layer) * N_dft + n] = c[0] * px[0][idx] - c[1] * px[1][idx];
				pDMRS[table_len + (slot * N_layer + layer) * N_dft + n] = c[0] * px[1][idx] + c[1] * px[0][idx];
			}
		}
	}

	if (N_layer == 2)
	{
		for (int n = 0; n < N_dft; n++)
		{
			//	(*(*(*(pDMRS+1)+1)+n))*=(-1.0);
			//	pDMRS[(1 * N_layer + 1) * N_dft + n] *= (-1.0);
			pDMRS[(1 * N_layer + 1) * N_dft + n] = -1.0 * pDMRS[(1 * N_layer + 1) * N_dft + n];
			pDMRS[table_len + (1 * N_layer + 1) * N_dft + n] = -1.0 * pDMRS[table_len + (1 * N_layer + 1) * N_dft + n];
		}
	}
	else
	{}
}

void geneDMRS(float *pDMRSReal, float *pDMRSImag, int N_layer, int N_dft)
{
	int i;
//	int table_len = 2 * N_layer * N_dft;
	
	int pPrimeTable[6][2];
	int ncs[2] = {3, 11};
	int Nzc;
	int zc_flag;
	int idx;
	int RSu, RSv;
	double qbar, q;
//	std::complex<double> px[1200];
	double px[2][1200];
	
	pPrimeTable[0][0] = 75;		pPrimeTable[0][1] = 73;
	pPrimeTable[1][0] = 150;	pPrimeTable[1][1] = 149;
	pPrimeTable[2][0] = 300;	pPrimeTable[2][1] = 293;
	pPrimeTable[3][0] = 600;	pPrimeTable[3][1] = 599;
	pPrimeTable[4][0] = 900;	pPrimeTable[4][1] = 887;
	pPrimeTable[5][0] = 1200;	pPrimeTable[5][1] = 1193;

	zc_flag = 1;
	idx = 0;
	while (zc_flag)
	{
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

	RSu = 0;
	RSv = 0;
	qbar = ((double)Nzc) * (RSu + 1.0) / 31.0;
	q = floor((qbar + 0.5)) + RSv * pow(-1.0, (floor((2.0 * qbar))));

	for (int m = 0; m < Nzc; m++)
	{
		double theta = (-PI * q * m * (m + 1.0)) / ((double)Nzc);
		//	px[m] = std::complex<double>(cos(theta), sin(theta));
		px[0][m] = cos(theta);
		px[1][m] = sin(theta);
	}

	for (int slot = 0; slot < 2; slot++)
	{
		for (int layer = 0; layer < N_layer; layer++)
		{
			int cs = ncs[slot] + 6 * layer;
			double alpha = (2.0 * PI * cs) / 12.0;

			for (int n = 0; n < N_dft; n++)
			{
				int idx = (n % Nzc);
				double c_real = cos(alpha * n);
				double c_imag = sin(alpha * n);
				//	std::complex<double> c = std::complex<double>((cos(alpha*n)),(sin(alpha*n)));

				pDMRSReal[(slot * N_layer + layer) * N_dft + n] = c_real * px[0][idx] - c_imag * px[1][idx];
				pDMRSImag[(slot * N_layer + layer) * N_dft + n] = c_real * px[1][idx] + c_imag * px[0][idx];
			}
		}
	}

	if (N_layer == 2)
	{
		for (int n = 0; n < N_dft; n++)
		{
			//	(*(*(*(pDMRS+1)+1)+n))*=(-1.0);
			//	pDMRS[(1 * N_layer + 1) * N_dft + n] *= (-1.0);
			pDMRSReal[(1 * N_layer + 1) * N_dft + n] = -1.0 * pDMRSReal[(1 * N_layer + 1) * N_dft + n];
			pDMRSImag[(1 * N_layer + 1) * N_dft + n] = -1.0 * pDMRSImag[(1 * N_layer + 1) * N_dft + n];
		}
	}
	else
	{}
}


