
#include "Modulation.h"

#include <stdio.h>

//#define N_MOD_IN

static const float INF = 1.0e9;

float BPSK_table[2][2];
float QPSK_table[4][2];
float QAM16_table[16][2];
float QAM64_table[64][2];

void init_mod_tables()
{
	int i, j;
	
	// BPSK
	BPSK_table[0][0] = +1.0; BPSK_table[0][1] = +1.0;
	BPSK_table[1][0] = -1.0; BPSK_table[1][1] = -1.0;
	
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			BPSK_table[i][j] /= sqrt(2.0);
		}
	}

	// QPSK
	QPSK_table[0][0] = +1.0; QPSK_table[0][1] = +1.0;
	QPSK_table[1][0] = +1.0; QPSK_table[1][1] = -1.0;
	QPSK_table[2][0] = -1.0; QPSK_table[2][1] = +1.0;
	QPSK_table[3][0] = -1.0; QPSK_table[3][1] = -1.0;
	
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 2; j++)
		{
			QPSK_table[i][j] /= sqrt(2.0);
		}
	}

	// QAM16
	QAM16_table[0][0] = +1.0; QAM16_table[0][1] = +1.0;
	QAM16_table[1][0] = +1.0; QAM16_table[1][1] = +3.0;
	QAM16_table[2][0] = +3.0; QAM16_table[2][1] = +1.0;
	QAM16_table[3][0] = +3.0; QAM16_table[3][1] = +3.0;
	QAM16_table[4][0] = +1.0; QAM16_table[4][1] = -1.0;
	QAM16_table[5][0] = +1.0; QAM16_table[5][1] = -3.0;
	QAM16_table[6][0] = +3.0; QAM16_table[6][1] = -1.0;
	QAM16_table[7][0] = +3.0; QAM16_table[7][1] = -3.0;
	QAM16_table[8][0] = -1.0; QAM16_table[8][1] = +1.0;
	QAM16_table[9][0] = -1.0; QAM16_table[9][1] = +3.0;
	QAM16_table[10][0] = -3.0; QAM16_table[10][1] = +1.0;
	QAM16_table[11][0] = -3.0; QAM16_table[11][1] = +3.0;
	QAM16_table[12][0] = -1.0; QAM16_table[12][1] = -1.0;
	QAM16_table[13][0] = -1.0; QAM16_table[13][1] = -3.0;
	QAM16_table[14][0] = -3.0; QAM16_table[14][1] = -1.0;
	QAM16_table[15][0] = -3.0; QAM16_table[15][1] = -3.0;
	
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 2; j++)
		{
			QAM16_table[i][j] /= sqrt(10.0);
		}
	}

	// QAM64
	QAM64_table[0][0] = +3.0; QAM64_table[0][1] = +3.0;
	QAM64_table[1][0] = +3.0; QAM64_table[1][1] = +1.0;
	QAM64_table[2][0] = +1.0; QAM64_table[2][1] = +3.0;
	QAM64_table[3][0] = +1.0; QAM64_table[3][1] = +1.0;
	QAM64_table[4][0] = +3.0; QAM64_table[4][1] = +5.0;
	QAM64_table[5][0] = +3.0; QAM64_table[5][1] = +7.0;
	QAM64_table[6][0] = +1.0; QAM64_table[6][1] = +5.0;
	QAM64_table[7][0] = +1.0; QAM64_table[7][1] = +7.0;

	QAM64_table[8][0] = +5.0; QAM64_table[8][1] = +3.0;
	QAM64_table[9][0] = +5.0; QAM64_table[9][1] = +1.0;
	QAM64_table[10][0] = +7.0; QAM64_table[10][1] = +3.0;
	QAM64_table[11][0] = +7.0; QAM64_table[11][1] = +1.0;
	QAM64_table[12][0] = +5.0; QAM64_table[12][1] = +5.0;
	QAM64_table[13][0] = +5.0; QAM64_table[13][1] = +7.0;
	QAM64_table[14][0] = +7.0; QAM64_table[14][1] = +5.0;
	QAM64_table[15][0] = +7.0; QAM64_table[15][1] = +7.0;

	QAM64_table[16][0] = +3.0; QAM64_table[16][1] = -3.0;
	QAM64_table[17][0] = +3.0; QAM64_table[17][1] = -1.0;
	QAM64_table[18][0] = +1.0; QAM64_table[18][1] = -3.0;
	QAM64_table[19][0] = +1.0; QAM64_table[19][1] = -1.0;
	QAM64_table[20][0] = +3.0; QAM64_table[20][1] = -5.0;
	QAM64_table[21][0] = +3.0; QAM64_table[21][1] = -7.0;
	QAM64_table[22][0] = +1.0; QAM64_table[22][1] = -5.0;
	QAM64_table[23][0] = +1.0; QAM64_table[23][1] = -7.0;

	QAM64_table[24][0] = +5.0; QAM64_table[24][1] = -3.0;
	QAM64_table[25][0] = +5.0; QAM64_table[25][1] = -1.0;
	QAM64_table[26][0] = +7.0; QAM64_table[26][1] = -3.0;
	QAM64_table[27][0] = +7.0; QAM64_table[27][1] = -1.0;
	QAM64_table[28][0] = +5.0; QAM64_table[28][1] = -5.0;
	QAM64_table[29][0] = +5.0; QAM64_table[29][1] = -7.0;
	QAM64_table[30][0] = +7.0; QAM64_table[30][1] = -5.0;
	QAM64_table[31][0] = +7.0; QAM64_table[31][1] = -7.0;

	QAM64_table[32][0] = -3.0; QAM64_table[32][1] = +3.0;
	QAM64_table[33][0] = -3.0; QAM64_table[33][1] = +1.0;
	QAM64_table[34][0] = -1.0; QAM64_table[34][1] = +3.0;
	QAM64_table[35][0] = -1.0; QAM64_table[35][1] = +1.0;
	QAM64_table[36][0] = -3.0; QAM64_table[36][1] = +5.0;
	QAM64_table[37][0] = -3.0; QAM64_table[37][1] = +7.0;
	QAM64_table[38][0] = -1.0; QAM64_table[38][1] = +5.0;
	QAM64_table[39][0] = -1.0; QAM64_table[39][1] = +7.0;

	QAM64_table[40][0] = -5.0; QAM64_table[40][1] = +3.0;
	QAM64_table[41][0] = -5.0; QAM64_table[41][1] = +1.0;
	QAM64_table[42][0] = -7.0; QAM64_table[42][1] = +3.0;
	QAM64_table[43][0] = -7.0; QAM64_table[43][1] = +1.0;
	QAM64_table[44][0] = -5.0; QAM64_table[44][1] = +5.0;
	QAM64_table[45][0] = -5.0; QAM64_table[45][1] = +7.0;
	QAM64_table[46][0] = -7.0; QAM64_table[46][1] = +5.0;
	QAM64_table[47][0] = -7.0; QAM64_table[47][1] = +7.0;

	QAM64_table[48][0] = -3.0; QAM64_table[48][1] = -3.0;
	QAM64_table[49][0] = -3.0; QAM64_table[49][1] = -1.0;
	QAM64_table[50][0] = -1.0; QAM64_table[50][1] = -3.0;
	QAM64_table[51][0] = -1.0; QAM64_table[51][1] = -1.0;
	QAM64_table[52][0] = -3.0; QAM64_table[52][1] = -5.0;
	QAM64_table[53][0] = -3.0; QAM64_table[53][1] = -7.0;
	QAM64_table[54][0] = -1.0; QAM64_table[54][1] = -5.0;
	QAM64_table[55][0] = -1.0; QAM64_table[55][1] = -7.0;

	QAM64_table[56][0] = -5.0; QAM64_table[56][1] = -3.0;
	QAM64_table[57][0] = -5.0; QAM64_table[57][1] = -1.0;
	QAM64_table[58][0] = -7.0; QAM64_table[58][1] = -3.0;
	QAM64_table[59][0] = -7.0; QAM64_table[59][1] = -1.0;
	QAM64_table[60][0] = -5.0; QAM64_table[60][1] = -5.0;
	QAM64_table[61][0] = -5.0; QAM64_table[61][1] = -7.0;
	QAM64_table[62][0] = -7.0; QAM64_table[62][1] = -5.0;
	QAM64_table[63][0] = -7.0; QAM64_table[63][1] = -7.0;
	
	for (i = 0; i < 64; i++)
	{
		for (j = 0; j < 2; j++)
		{
			QAM64_table[i][j] /= sqrt(42.0);
		}
	}
}

void Demodulating(/*LTE_PHY_PARAMS *lte_phy_params, */float pDecSeq[N_MOD_OUT_MAX * 2], float pLLR[N_MOD_IN_MAX], int in_buf_sz, int mod_type, float awgnSigma)
{
#pragma HLS ARRAY_PARTITION variable=pLLR cyclic factor=4 dim=1
#pragma HLS ARRAY_PARTITION variable=QAM16_table cyclic factor=16 dim=1
//#pragma HLS RESOURCE variable=QAM16_table core=RAM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable=pDecSeq cyclic factor=16 dim=1
//#pragma HLS RESOURCE variable=pDecSeq core=RAM_2P_BRAM

	float No = 2.0 * (pow(awgnSigma, 2.0));
	int bits_per_samp;
	int mod_table_len;

	int idx_table[MAX_MOD_TABLE_LEN][MAX_MOD_BITS_PER_SAMP];
	float metric[MAX_MOD_TABLE_LEN];
//#pragma HLS ARRAY_PARTITION variable=metric block factor=16 dim=1
//#pragma HLS RESOURCE variable=metric core=RAM_2P_BRAM
	float metric_set[2][(MAX_MOD_TABLE_LEN / 2)];

//	printf("in_buf_sz = %d\n", in_buf_sz);

//	int in_buf_sz;

	int i, j, k;

//	in_buf_sz = lte_phy_params->demod_in_buf_sz;

//	set_mod_params(&p_table, &bits_per_samp, &mod_table_len, mod_type);
	init_mod_tables();

	bits_per_samp = QAM16_BITS_PER_SAMP;
	mod_table_len = QAM16_TABLE_LEN;
	
	for (i = 0; i < mod_table_len /*QAM16_BITS_PER_SAMP*/; i++)
	{
		for (j = 0; j < bits_per_samp /*QAM16_TABLE_LEN*/; j++)
		{
			idx_table[i][j] = 0;
		}
	}
	for (i = 0; i < mod_table_len /*QAM16_TABLE_LEN*/; i++)
	{ 
 
		int idx_val = i;
		int b = bits_per_samp - 1;
		
		while (idx_val)
		{
			idx_table[i][b] = idx_val % 2;
			idx_val /= 2;
			b--;
		}
	}
//	printf("%d\n", in_buf_sz);
	
	DEMOD_SYMB_LOOP: for (i = 0; i < in_buf_sz /*N_MOD_IN*/; i++)
	{
#pragma HLS PIPELINE
		DEMOD_TABLE_LOOP1: for(j = 0; j < mod_table_len; j++)
		{
//#pragma HLS UNROLL
			float tmp[2];
//#pragma HLS RESOURCE variable=tmp core=RAM_2P_BRAM
			
			tmp[0] = pDecSeq[2 * i + 0] - QAM16_table[j][0];
			tmp[1] = pDecSeq[2 * i + 1] - QAM16_table[j][1];
			metric[j] = tmp[0] * tmp[0] + tmp[1] * tmp[1];
		}

		DEMOD_BITS_LOOP: for (j = 0; j < bits_per_samp; j++)
		{
//#pragma HLS UNROLL
			float min0, min1;
		//	int idx0 = 0, idx1 = 0;
			int idx[2] = {0, 0};

			DEMOD_TABLE_LOOP2: for (k = 0; k < mod_table_len; k++)
			{
//#pragma HLS UNROLL
				/*
				if(idx_table[k][j] == 0)
				{
					metric_set[0][idx0] = metric[k];
					idx0++;
				}
				else
				{
					metric_set[1][idx1] = metric[k];
					idx1++;
				}*/
				int cnt = idx_table[k][j];
				metric_set[cnt][idx[cnt]] = metric[k];
				idx[cnt]++;
			}

		//	min0 = vecmin(metric_set[0], mod_table_len / 2);
			min0 = metric_set[0][0];
			MIN_LOOP0: for (k = 1; k < mod_table_len / 2; k++)
			{
				if (metric_set[0][k] < min0)
					min0 = metric_set[0][k];
			}
		//	min1  = vecmin(metric_set[1], mod_table_len / 2);
			min1 = metric_set[1][0];
			MIN_LOOP1: for (k = 1; k < mod_table_len / 2; k++)
			{
				if (metric_set[1][k] < min0)
					min1 = metric_set[1][k];
			}

			if (No == (float)0)
			{
				pLLR[i * bits_per_samp + j] = (min0 - min1);
			}
			else
			{
				pLLR[i * bits_per_samp + j] = (min0 - min1) / No;
			}
		}
	}
}

