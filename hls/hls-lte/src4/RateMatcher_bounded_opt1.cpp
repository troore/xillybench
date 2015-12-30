
#include <stdio.h>
#include "RateMatcher.h"

#define N_RX_ANTENNA 2
#define N_RM_OUT (2 * LTE_PHY_DFT_SIZE_30_72MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * QAM16_BITS_PER_SAMP)
#define N_RM_IN N_RM_OUT


void RxRateMatching(float pLLRin[/*N_RM_OUT_MAX*/ N_RM_OUT], float pLLRout[/*N_RM_IN_MAX*/ N_RM_IN], int pHD[/*N_RM_IN_MAX*/ N_RM_IN], int out_buf_sz)
{
	int n_blocks;
	int rm_blk_sz;
	int rm_data_length;
	int rm_last_blk_len;
	int out_block_offset;
	int cur_blk_len;
//	int out_buf_sz;
	
	float pInMatrix[RATE * (BLOCK_SIZE + 4)] = {0.0};
	float pOutMatrix[RATE * (BLOCK_SIZE + 4)] = {0.0};

	int i, j, r;

//	out_buf_sz = lte_phy_params->rdm_out_buf_sz;
	rm_blk_sz = BLOCK_SIZE + 4;
	rm_data_length = (out_buf_sz / RATE);

	n_blocks = (rm_data_length + (rm_blk_sz - 1)) / rm_blk_sz;

	if (rm_data_length % rm_blk_sz)
	{
		rm_last_blk_len = (rm_data_length % rm_blk_sz);
	}
	else
	{
		rm_last_blk_len = rm_blk_sz;
	}


	out_block_offset = 0;

#define OUT_BUF_SZ 110664
#define N_BLOCKS 6
#define CUR_BLK_LEN 6148

//	printf("%d %d %d %d %d\n", out_buf_sz, n_blocks, rm_blk_sz, rm_last_blk_len, N_RM_IN);
	
	DeTransposition_loop:for (i = 0; i < /*n_blocks*/ N_BLOCKS; i++)
	{
		cur_blk_len = (i != (n_blocks - 1)) ? rm_blk_sz : rm_last_blk_len;
		
		DeTransposition_inner_loop1:for (j = 0; j < /*cur_blk_len*/ CUR_BLK_LEN; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE II=4
				pInMatrix[r * cur_blk_len + j] = pLLRin[out_block_offset + RATE * j + r];
			}
		}  

		SubblockDeInterleaving(cur_blk_len, pInMatrix, pOutMatrix);

		DeTransposition_inner_loop2:for (j = 0; j < /*cur_blk_len*/ CUR_BLK_LEN; j++)
		{
			for (r = 0; r < RATE; r++)
			{
#pragma HLS PIPELINE II=4
				pLLRin[out_block_offset + RATE * j + r] = pOutMatrix[r * cur_blk_len + j];
			}
		}

		out_block_offset += RATE * cur_blk_len;
	}

	pHD_pos_neg:for (i = 0; i < /*out_buf_sz*/ OUT_BUF_SZ; i++)
	{
#pragma HLS PIPELINE II=4
		if (pLLRin[i] < 0)
		{
			pHD[i] = 0;
		}
		else
		{
			pHD[i] = 1;
		}
	}

	pLLRout_assign:for (i = 0; i < /*out_buf_sz*/ OUT_BUF_SZ; i++)
	{
#pragma HLS PIPELINE II=4
		pLLRout[i] = pLLRin[i];
	}
}

