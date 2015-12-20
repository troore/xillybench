
#include "Scrambler.h"

#define N_SCRAMB_IN	(2 * LTE_PHY_DFT_SIZE_1_92MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * QAM16_BITS_PER_SAMP)
#define N_SCRAMB_OUT N_SCRAMB_IN


void Descrambling(/*LTE_PHY_PARAMS *lte_phy_params, */float pInpSeq[/*N_SCRAMB_IN_MAX*/ N_SCRAMB_IN], float pOutSeq[/*N_SCRAMB_OUT_MAX*/ N_SCRAMB_OUT], int n)
{
//	int n_inp;
	float scramb_seq_float[N_SCRAMB_IN_MAX];
	int scramb_seq_int[N_SCRAMB_IN_MAX];

	int i;

	printf("%d %d\n", N_SCRAMB_IN, n);

//	n_inp = lte_phy_params->scramb_in_buf_sz;
	// Generate integer scrambling sequence
	GenScrambInt(scramb_seq_int, n);

	/*
	for (i = 0; i < n_inp; i++)
	{
		if (1 == scramb_seq_int[i])
		{
			scramb_seq_float[i] = -1.0;
		}
		else
		{
			scramb_seq_float[i] = +1.0;
		}
	}
	for (i = 0; i < n_inp; i++)
		pOutSeq[i] = pInpSeq[i] * scramb_seq_float[i];
	*/

	for (i = 0; i < /*n*/ N_SCRAMB_IN; i++)
	{
#pragma HLS PIPELINE II=4
		pOutSeq[i] = (pInpSeq[i] * (scramb_seq_int[i] * (-2.0) + 1.0));
	}
}

void GenScrambInt(int pScrambInt[/*N_SCRAMB_IN_MAX*/ N_SCRAMB_IN], int n)
{
	int i;
	int N_c = 1600;
	
	int n_init[31] = { 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0};

	/////////////////////Generate ScrambSeq///////////////////////
	int px1[N_SCRAMB_IN + 1600];
	int px2[N_SCRAMB_IN + 1600];


	for (i = 0; i < 31; i++)
	{
#pragma HLS PIPELINE II=4
		px1[i] = 0;
		px2[i] = n_init[i];
	}
	px1[0] = 1;


	/*
	GenScramLoop1(px1, px2);
	GenScramLoop2(px1, px2, n);
	GenScramLoop3(px1, px2, pScrambInt, n);
	*/

	int px2_tmp0, px2_tmp1, px2_tmp2, px2_tmp3, px1_tmp0, px1_tmp1, px1_tmp2, px1_tmp3, px1_tmp4, px1_tmp5;
	px2_tmp0 = px2[0];
	px2_tmp1 = px2[1];
	px2_tmp2 = px2[2];
	px1_tmp0 = px1[0];
	px1_tmp1 = px1[1];
	px1_tmp2 = px1[2];

	GenScrambInt_label2:for (i = 0; i < /*n*/N_SCRAMB_IN + N_c - 31; i++)
	{
#pragma HLS PIPELINE II=4
//		if(i >= n + N_c -31)
//			break;
		px1_tmp3 = px1[i + 3];
		px1[i + 31] = (px1_tmp3 + px1_tmp0) % 2;
		px1_tmp0 = px1_tmp1;
		px1_tmp1 = px1_tmp2;
		px1_tmp2 = px1_tmp3;

		px2_tmp3 = px2[i + 3];
		px2[i + 31] = (px2_tmp3 + px2_tmp2 + px2_tmp1 + px2_tmp0) % 2;
		px2_tmp0 = px2_tmp1;
		px2_tmp1 = px2_tmp2;
		px2_tmp2 = px2_tmp3;
	}
	GenScrambInt_label3:for (i = 0; i < /*n*/N_SCRAMB_IN; i++)
	{
#pragma HLS PIPELINE II=4
//		if(i >= n)
//			break;
		pScrambInt[i] = (px1[i + N_c] + px2[i + N_c]) % 2;
	}

	/////////////////////END Generate ScrambSeq///////////////////////
}

