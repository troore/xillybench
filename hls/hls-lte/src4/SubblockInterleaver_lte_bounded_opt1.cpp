
#include <stdio.h>
#include "SubblockInterleaver_lte.h"

static int InterColumnPattern[32] = {0,16,8,24,4,20,12,28,
									2,18,10,26,6,22,14,30,
									1,17,9,25,5,21,13,29,3,
									19,11,27,7,23,15,31};
static int Rate = 3, C_sb = 32, DummyValue = 1000000;

void SubblockDeInterleaving(int SeqLen, float pInpMtr[RATE * (BLOCK_SIZE + 4)], float pOutMtr[RATE * (BLOCK_SIZE + 4)])
{
	int D;
	int K_pi;
	int Rate;
	int R_sb, C_sb;
	int NumDummy;
	float DummyValue;
	
	int InIdx;
	int OutIdx;

	D = SeqLen;
	Rate = 3;
	C_sb = 32;
	
	R_sb = (D + (C_sb - 1)) / C_sb;
	
	K_pi = R_sb * C_sb;
	NumDummy = K_pi - D;
	DummyValue = (float)1000000;


#define R_SB 193
#define C_SB 32
#define K_PI 6176
#define	NUM_DUMMY 28
	
//////////////////// DeInterleaving for i=0,1 ///////////////////////
	float pInterMatrix[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0.0};
#pragma HLS DEPENDENCE variable=pInterMatrix inter false

	for (int StrIdx = 0; StrIdx < (/*Rate*/ RATE - 1); StrIdx++)
	{
		InIdx=0;
		inter_matrix_assign:for (int r = 0;r < /*R_sb*/ R_SB; r++)
		{
			for (int c = 0; c < /*C_sb*/ C_SB; c++)
			{
#pragma HLS PIPELINE II=4
				int k = r * C_sb + c;
				
				if (k < NumDummy)
				{
					pInterMatrix[r * C_sb + c] = DummyValue;
				}
				else
				{
					pInterMatrix[r * C_sb + c] = (float)0;
				}
			}
		}

		from_input_to_inter:for(int c = 0; c < /*C_sb*/ C_SB; c++)
		{

			for (int r = 0; r < /*R_sb*/ R_SB; r++)
			{
#pragma HLS PIPELINE II=4
				int col = InterColumnPattern[c];
				int k = col * R_sb + r;
				//float v = pInterMatrix[r * C_sb + col];
				
				if (pInterMatrix[r * C_sb + col] != DummyValue)
				{
					pInterMatrix[r * C_sb + col] = pInpMtr[StrIdx * D + InIdx];
					InIdx++;
				}  
			}
		}  

		OutIdx=0;
		from_inter_to_output:for (int r = 0; r < /*R_sb*/ R_SB; r++)
		{
			for(int c = 0; c < /*C_sb*/ C_SB; c++)
			{
#pragma HLS PIPELINE II=4
				float v = pInterMatrix[r * C_sb + c];

				if (v == DummyValue)
				{}
				else
				{
					pOutMtr[StrIdx * D + OutIdx] = pInterMatrix[r * C_sb + c];
					OutIdx++;
				}
			}
		}
	}

//////////////////// DeInterleaving for i=2 ///////////////////////
	int Pi[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0};
	float pInterSeq[((BLOCK_SIZE + 4 + 31) / 32) * 32] = {0.0};
#pragma HLS DEPENDENCE variable=pInterSeq inter false
	
	rate3_dummyvalue_assign:for (int k = 0; k < /*NumDummy*/ NUM_DUMMY; k++)
		pInterSeq[k] = DummyValue;
//////////////// Pi & DePi//////////////////
	calculate_index:for(int k=0;k</*K_pi*/ K_PI;k++)
	{
#pragma HLS PIPELINE II=4
		int idxP = k / R_sb;
		int idx = (InterColumnPattern[idxP] + (C_sb * (k % R_sb)) + 1) % K_pi;
		
		Pi[k]=idx;
	}
/////////////// DeInterleaving ////////////////////
	InIdx=0;
	rate3_from_input_to_inter:for(int k=0;k</*K_pi*/ K_PI;k++)
	{
#pragma HLS PIPELINE II=4
		//float v = pInterSeq[Pi[k]];
		if (pInterSeq[Pi[k]] != DummyValue)
		{
			pInterSeq[Pi[k]] = pInpMtr[(Rate - 1) * D + InIdx];
			InIdx++;
		}
	}
	OutIdx=0;
	from_inter_to_out:for (int k = /*NumDummy*/ NUM_DUMMY; k < /*K_pi*/ K_PI; k++)
	{
#pragma HLS PIPELINE II=4
		pOutMtr[(Rate - 1) * D + OutIdx] = pInterSeq[k];
		OutIdx++;
	}
}

