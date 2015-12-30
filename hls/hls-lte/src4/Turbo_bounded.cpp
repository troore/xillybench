#include <stdio.h>

#include "Turbo.h"

#define LOG_INFINITY 1e30

#ifdef BLOCK_SIZE
#undef BLOCK_SIZE
#define BLOCK_SIZE 6144
#endif
#ifdef N_UNCODED
#undef N_UNCODED
#define N_UNCODED BLOCK_SIZE
#endif


/**************Global variables*****************/

int InpBlockShift;
int OutBlockShift;
int NumBlock;
int DataLength;
int LastBlockLength;
	
/* Turbo specific parameters */
int g_gens[N_GENS] = {013, 015};

int g_gen_pols[N_GENS];
int g_rev_gen_pols[N_GENS];
int g_state_trans[N_STATES * 2];
int g_rev_state_trans[N_STATES * 2];
int g_output_parity[N_STATES * (N_GENS - 1) * 2];
int g_rev_output_parity[N_STATES * (N_GENS - 1) * 2];

float Lc;

/*****************End of globals*****************/

float max_log(float a, float b)
{
	return (a > b) ? a : b;
}

void set_generator_polynomials(int gens[N_GENS], int n_gens /*it is 2*/, int constraint_length)
{
	int i, j;
	int K = constraint_length;
	int m = K - 1;
	int n_states = (1 << m);
	int s0, s1, s_prim;
	
	int p0[N_GENS - 1];
	int p1[N_GENS - 1];

	set_generator_polynomials_loop1:for (i = 0; i < /*n_gens*/ N_GENS; i++)
	{
#pragma HLS UNROLL
		if(i >= n_gens)
			break;
		g_gen_pols[i] = gens[i];
		g_rev_gen_pols[i] = reverse_int(K, gens[i]);
	}

	set_generator_polynomials_loop2:for (s_prim = 0; s_prim < N_STATES; s_prim++)
	{
#pragma HLS UNROLL
//		std::cout << s_prim << std::endl;
		s0 = calc_state_transition(s_prim, 0, p0);
//		std::cout << s0 << "\t";
		g_state_trans[s_prim * 2 + 0] = s0;
		g_rev_state_trans[s0 * 2 + 0] = s_prim;
		set_generator_polynomials_loop2_in1:for (j = 0; j < (/*n_gens*/ N_GENS - 1); j++)
		{
			g_output_parity[s_prim * (n_gens - 1) * 2 + 2 * j + 0] = p0[j];
			g_rev_output_parity[s0 * (n_gens - 1) * 2 + 2 * j + 0] = p0[j];
		}

		s1 = calc_state_transition(s_prim, 1, p1);
//		std::cout << s1 << std::endl;
		g_state_trans[s_prim * 2 + 1] = s1;
		g_rev_state_trans[s1 * 2 + 1] = s_prim;
		set_generator_polynomials_loop2_in2:for (j = 0; j < (/*n_gens*/ N_GENS - 1); j++)
		{
			g_output_parity[s_prim * (n_gens - 1) * 2 + 2 * j + 1] = p1[j];
			g_rev_output_parity[s1 * (n_gens - 1) * 2 + 2 * j + 1] = p1[j];
		}
//		std::cout << (int)p0[0] << "\t" << (int)p1[0] << std::endl;
	}
}


#define M 6144

//template <typename T>
//void internal_interleaver(T in[BLOCK_SIZE], T out[BLOCK_SIZE], int m)
void internal_interleaver(int in[BLOCK_SIZE], int out[BLOCK_SIZE], int m)
{
    int i;
    int f1 = 0;
    int f2 = 0;
    int idx;

    // Determine f1 and f2
    for(i = 0; i < TURBO_INT_K_TABLE_SIZE; i++)
    {
        if(m == TURBO_INT_K_TABLE[i])
        {
            f1 = TURBO_INT_F1_TABLE[i];
            f2 = TURBO_INT_F2_TABLE[i];
            break;
        }
    }

    for(i = 0; i < m; i++)
    {
		if ((0 == f1) && (0 == f2))
		{
			idx = i;
		}
		else
		{
			//	idx = ((f1 * i + f2 * i * i) % m);
			idx = (((f1 % m) * (i % m)) % m + ((((f2 % m) * (i % m)) % m) * (i % m)) % m) % m; 
		}
        out[i] = in[idx];
    }
}

void internal_interleaver(float in[BLOCK_SIZE], float out[BLOCK_SIZE], int m)
{
    int i;
    int f1 = 0;
    int f2 = 0;
    int idx;

    // Determine f1 and f2
    for(i = 0; i < TURBO_INT_K_TABLE_SIZE; i++)
    {
#pragma HLS UNROLL
        if(m == TURBO_INT_K_TABLE[i])
        {
            f1 = TURBO_INT_F1_TABLE[i];
            f2 = TURBO_INT_F2_TABLE[i];
            break;
        }
    }

    for(i = 0; i < m /*M*/; i++)
    {
#pragma HLS PIPELINE
		if ((0 == f1) && (0 == f2))
		{
			idx = i;
		}
		else
		{
			idx = (((f1 % m) * (i % m)) % m + ((((f2 % m) * (i % m)) % m) * (i % m)) % m) % m; 
		}
        out[i] = in[idx];
    }
}

//template <typename T>
//void internal_deinterleaver(T in[BLOCK_SIZE], T out[BLOCK_SIZE], int m)
void internal_deinterleaver(int in[BLOCK_SIZE], int out[BLOCK_SIZE], int m)
{
    int i;
    int f1 = 0;
    int f2 = 0;
    int idx;

    // Determine f1 and f2
    for (i = 0; i < TURBO_INT_K_TABLE_SIZE; i++)
    {
        if (m == TURBO_INT_K_TABLE[i])
        {
            f1 = TURBO_INT_F1_TABLE[i];
            f2 = TURBO_INT_F2_TABLE[i];
            break;
        }
    }

    for (i = 0; i < m; i++)
    {
		if ((0 == f1) && (0 == f2))
		{
			idx = i;
		}
		else
		{
			//	idx = ((f1 * i + f2 * i * i) % m);
			idx = (((f1 % m) * (i % m)) % m + ((((f2 % m) * (i % m)) % m) * (i % m)) % m) % m; 
		}
        out[idx] = in[i];
    }
}

void internal_deinterleaver(float in[BLOCK_SIZE], float out[BLOCK_SIZE], int m)
{
    int i;
    int f1 = 0;
    int f2 = 0;
    int idx;

    // Determine f1 and f2
    for (i = 0; i < TURBO_INT_K_TABLE_SIZE; i++)
    {
#pragma HLS UNROLL
        if (m == TURBO_INT_K_TABLE[i])
        {
            f1 = TURBO_INT_F1_TABLE[i];
            f2 = TURBO_INT_F2_TABLE[i];
            break;
        }
    }

    for (i = 0; i < m /*M*/; i++)
    {
#pragma HLS PIPELINE
		if ((0 == f1) && (0 == f2))
		{
			idx = i;
		}
		else
		{
			idx = (((f1 % m) * (i % m)) % m + ((((f2 % m) * (i % m)) % m) * (i % m)) % m) % m; 
		}
        out[idx] = in[i];
    }
}

void constituent_encoder(int input[BLOCK_SIZE], int input_len, int tail[N_TAIL], int parity[(N_UNCODED + N_TAIL) * (N_GENS - 1)])
{
	int i, j;
	int encoder_state = 0, target_state;

	for (i = 0; i < input_len; i++)
	{
		for (j = 0; j < (N_GENS - 1); j++)
		{
			parity[i * (N_GENS - 1) + j] = g_output_parity[encoder_state * (N_GENS - 1) * 2 + j * 2 + input[i]];
		}
		encoder_state = g_state_trans[encoder_state * 2 + input[i]];
	}
	// add tail bits to make target_state to be 0
	for (i = 0; i < CST_LEN - 1; i++)
	{
		// Attend the shift direction!!!
		target_state = (encoder_state >> 1) & ((1 << (CST_LEN - 1)) - 1);
		if (g_state_trans[encoder_state * 2 + 0] == target_state)
		{
			tail[i] = 0;
		}
		else
		{
			tail[i] = 1;
		}
		for (j = 0; j < (N_GENS - 1); j++)
		{
			parity[(i + input_len) * (N_GENS - 1) + j] = g_output_parity[encoder_state * (N_GENS - 1) * 2 + j * 2 + tail[i]];
		}
		encoder_state = target_state;
	}
}

void turbo_encoding(int piSeq[N_TURBO_IN_MAX], int pcSeq[N_TURBO_OUT_MAX], int input_data_length)
{
//	int input_data_length;
	int n_blocks;
	int last_block_length;
	int cur_blk_len;
	int inp_blk_offset, out_bit_offset;
	int i, j, k;
	
	int input_bits[N_UNCODED];
	int interleaved_input_bits[N_UNCODED];
	int syst1[N_UNCODED + N_TAIL];
	int syst2[N_UNCODED + N_TAIL];
	int tail1[N_TAIL];
	int tail2[N_TAIL];
	int parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)];
	int parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)];

	set_generator_polynomials(g_gens, N_GENS, CST_LEN);

//	input_data_length = lte_phy_params->te_in_buf_sz;
	n_blocks = ((input_data_length + BLOCK_SIZE - 1) / BLOCK_SIZE);
	if (input_data_length % BLOCK_SIZE)
	{
		last_block_length = (input_data_length % BLOCK_SIZE);
	}
	else
	{
		last_block_length = BLOCK_SIZE;
	}

	inp_blk_offset = 0;
	out_bit_offset = 0;

	// encode all code blocks
	for (i = 0; i < n_blocks; i++)
	{
		cur_blk_len = (i != (n_blocks - 1)) ? BLOCK_SIZE : last_block_length;

		// encode one code block
		for (j = 0; j < cur_blk_len; j++)
		{
			input_bits[j] = piSeq[inp_blk_offset + j];
		}
		
		encode_block(input_bits, interleaved_input_bits, parity1, tail1, parity2, tail2, cur_blk_len);
		
//		memcpy(syst1, input_bits, sizeof(uint8_T) * m_n_uncoded);
//		memcpy(syst2, interleaved_input_bits, sizeof(uint8_T) * m_n_uncoded);
		for (j = 0; j < cur_blk_len; j++)
		{
			syst1[j] = input_bits[j];
			syst2[j] = interleaved_input_bits[j];
		}
//		memcpy(syst1 + m_n_uncoded, tail1, sizeof(uint8_T) * m_n_tail);
//		memcpy(syst2 + m_n_uncoded, tail2, sizeof(uint8_T) * m_n_tail);

		for (j = cur_blk_len; j < cur_blk_len + N_TAIL; j++)
		{
			syst1[j] = tail1[j - cur_blk_len];
			syst2[j] = tail2[j - cur_blk_len];
		}
		
		// the data part
		for (j = 0; j < cur_blk_len; j++)
		{
			// systematic bits
			pcSeq[out_bit_offset++] = syst1[j];
			// first parity bits
			for (k = 0; k < N_GENS - 1; k++)
			{
				pcSeq[out_bit_offset++] = parity1[j * (N_GENS - 1) + k];
			}
			// second parity bits
			for (k = 0; k < (N_GENS - 1); k++)
			{
				pcSeq[out_bit_offset++] = parity2[j * (N_GENS - 1) + k];
			}
		}
		
		// the first tail
		for (j = 0; j < N_TAIL; j++)
		{
			// first systematic tail bits
			pcSeq[out_bit_offset++] = syst1[cur_blk_len + j];
			// first parity tail bits
			for (k = 0; k < (N_GENS - 1); k++)
			{
				pcSeq[out_bit_offset++] = parity1[(cur_blk_len + j) * (N_GENS - 1) + k];
			}
		}

		// the second tail
		for (j = 0; j < N_TAIL; j++)
		{
			// second systematic tail bits
			pcSeq[out_bit_offset++] = syst2[cur_blk_len + j];
			// second parity tail bits
			for (k = 0; k < (N_GENS - 1); k++)
			{
				pcSeq[out_bit_offset++] = parity2[(cur_blk_len + j) * (N_GENS - 1) + k];
			}
		}

		inp_blk_offset += cur_blk_len;
	}
}

void encode_block(int input_bits[BLOCK_SIZE],
		int interleaved_input_bits[BLOCK_SIZE],
		int parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)], 
		int tail1[N_TAIL],
		int parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		int tail2[N_TAIL], 
		int blk_len)
{

	// the first encoder
	constituent_encoder(input_bits, blk_len, tail1, parity1);

	// the interleaver
	internal_interleaver(input_bits, interleaved_input_bits, blk_len);

	// the second encoder
	constituent_encoder(interleaved_input_bits, blk_len, tail2, parity2);
}

int calc_state_transition(int instate, int input, int parity[N_GENS - 1])
{
	int in = 0;
	int temp = (g_gen_pols[0] & instate), parity_temp, parity_bit;
	int i, j;

	calc_state_transition_loop1:for (i = 0; i < CST_LEN; i++)
	{
		in = (temp & 1) ^ in;
		temp = temp >> 1;
	}
	in = in ^ input;

	calc_state_transition_loop2:for (j = 0; j < (N_GENS - 1); j++)
	{
		parity_temp = (instate | (in << (CST_LEN - 1))) & g_gen_pols[j + 1];
		parity_bit = 0;
		calc_state_transition_loop2_in:for (i = 0; i < CST_LEN; i++)
		{
			parity_bit = (parity_temp & 1) ^ parity_bit;
			parity_temp = parity_temp >> 1;
		}
		parity[j] = parity_bit;
	}
	
	return (in << (CST_LEN - 2) | (instate >> 1)) & ((1 << (CST_LEN - 1)) - 1);
}

int reverse_int(int length, int in)
{
	int out = 0;
	int i, j;

	for (i = 0; i < /*(length >> 1)*/ 2; i++)
	{
		out = out | ((in & (1 << i)) << (length - 1 - (i << 1)));
	}
	for (j = 0; j < /*(length - i)*/ 2; j++)
	{
		out = out | ((in & (1 << (j + i))) >> ((j << 1) - (length & 1) + 1));
	}
	
	return out;
}

/*
 * FIXME: How to determine output buffer size from
 * input buffer size? But it doesn't matter for now...
 */
#define N_DFT LTE_PHY_DFT_SIZE_30_72MHZ 
#define N_TURBO_OUT	(2 * N_DFT * 12 * 4)
#define N_TURBO_IN	(N_TURBO_OUT / RATE)

void turbo_decoding(float pInpData[/*N_TURBO_OUT_MAX*/ N_TURBO_OUT], int pOutBits[/*N_TURBO_IN_MAX*/ N_TURBO_IN], int out_data_length, int n_iters)
{
//#pragma HLS DATAFLOW
#pragma HLS ARRAY_PARTITION variable=pInpData cyclic factor=3 dim=1
//	int out_data_length;

	int n_blocks;
	int n_blocks_max;
	int last_block_length;
	int out_bit_offset, out_block_offset;
	int cur_blk_len;

	float recv_syst1[N_UNCODED + N_TAIL];
	float recv_syst2[N_UNCODED + N_TAIL];
	float recv_parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)];
	float recv_parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)];

	/*float recv_syst1[(N_UNCODED + N_TAIL) * 3];
		float recv_syst2[(N_UNCODED + N_TAIL) * 3];
		float recv_parity1[((N_UNCODED + N_TAIL) * (N_GENS - 1)) * 3];
		float recv_parity2[((N_UNCODED + N_TAIL) * (N_GENS - 1)) * 3];*/

	int i, j, k;

	set_generator_polynomials(g_gens, N_GENS, CST_LEN);

//	out_data_length = lte_phy_params->td_out_buf_sz;
//  according to lte_phy.cpp, maximum of td_out_buf_sz is out_data_length (72 MHz).

//	printf("BLOCK_SIZE=%d\n", BLOCK_SIZE);

	n_blocks = ((out_data_length + BLOCK_SIZE - 1) / BLOCK_SIZE);
//	printf("%d\n", n_blocks);
#define N_BLOCKS 6

	if (out_data_length % BLOCK_SIZE)
	{
		last_block_length = (out_data_length % BLOCK_SIZE);
	}
	else
	{
		last_block_length = BLOCK_SIZE;
	}

//	printf("%d %d\n", out_data_length, last_block_length);

	// Clear data part of recv_syst2
	for (i = 0; i < N_UNCODED; i++)
	{
#pragma HLS UNROLL
		recv_syst2[i] = 0.0;
	}

	out_bit_offset = 0;
	out_block_offset = 0;

#define CUR_BLK_LEN 6144

		for (i = 0; i < /*n_blocks*/ N_BLOCKS; i++)
		{
			cur_blk_len = (i != (n_blocks - 1)) ? BLOCK_SIZE : last_block_length;

//			printf("cur_blk_len=%d\n", cur_blk_len);
			// data part
			for (j = 0; j < CUR_BLK_LEN /*cur_blk_len*/; j++)
			{
	#pragma HLS PIPELINE
				/*
				if(j >= cur_blk_len)
					break;
					*/
				recv_syst1[j] = pInpData[out_bit_offset++];
				for (k = 0; k < N_GENS - 1; k++)
				{
					recv_parity1[j * (N_GENS - 1) + k] = pInpData[out_bit_offset++];
				}
				for (k = 0; k < N_GENS - 1; k++)
				{
					recv_parity2[j * (N_GENS - 1) + k] = pInpData[out_bit_offset++];
				}
			}
			// first tail
			for (j = 0; j < N_TAIL; j++)
			{
	#pragma HLS PIPELINE
				recv_syst1[cur_blk_len + j] = pInpData[out_bit_offset++];
				for (k = 0; k < N_GENS - 1; k++)
				{
					recv_parity1[(cur_blk_len + j) * (N_GENS - 1) + k] = pInpData[out_bit_offset++];
				}
			}
			// second tail
			for (j = 0; j < N_TAIL; j++)
			{
	#pragma HLS PIPELINE
				recv_syst2[cur_blk_len + j] = pInpData[out_bit_offset++];
				for (k = 0; k < N_GENS - 1; k++)
				{
					recv_parity2[(cur_blk_len + j) * (N_GENS - 1) + k] = pInpData[out_bit_offset++];
				}
			}

			decode_block(recv_syst1, recv_syst2, recv_parity1, recv_parity2, pOutBits + out_block_offset, cur_blk_len, n_iters);
			out_block_offset += cur_blk_len;
		}
}

void decode_block(float recv_syst1[N_UNCODED + N_TAIL], 
		float recv_syst2[N_UNCODED + N_TAIL],
		float recv_parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		float recv_parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		int decoded_bits_i[BLOCK_SIZE], 
		int interleaver_size,
		int n_iters)
{
	int n_tailed = interleaver_size + N_TAIL;
	int i;
	int j;
	
	float Le12[BLOCK_SIZE + N_TAIL];
	float Le12_int[BLOCK_SIZE + N_TAIL];
	float Le21[BLOCK_SIZE + N_TAIL];
	float Le21_int[BLOCK_SIZE + N_TAIL];
	float L[BLOCK_SIZE + N_TAIL];

	float int_recv_syst1[BLOCK_SIZE + N_TAIL];
	float deint_recv_syst2[BLOCK_SIZE + N_TAIL];
	float recv_syst[BLOCK_SIZE + N_TAIL];
	float int_recv_syst[BLOCK_SIZE + N_TAIL];

//	memset(Le21, 0, n_tailed * sizeof(float));
#define N_TAILED 6147
//	printf("n_tailed=%d\n", n_tailed);
	for (i = 0; i < /*n_tailed*/ N_TAILED; i++)
		Le21[i] = 0.0;

#define INTERLEAVER_SIZE 6144
//	printf("interleaver_size=%d\n", interleaver_size);
	internal_interleaver(recv_syst1, int_recv_syst1, interleaver_size);
	internal_deinterleaver(recv_syst2, deint_recv_syst2, interleaver_size);

	// Combine the results from recv_syst1 and recv_syst2 (in case some bits are transmitted several times)
	for (i = 0; i < /*interleaver_size*/ INTERLEAVER_SIZE; i++)
	{
#pragma HLS PIPELINE
		recv_syst[i] = recv_syst1[i] + deint_recv_syst2[i];
	}
	for (i = 0; i < /*interleaver_size*/ INTERLEAVER_SIZE; i++)
	{
#pragma HLS PIPELINE
		int_recv_syst[i] = recv_syst2[i] + int_recv_syst1[i];
	}
	for (i = /*interleaver_size*/ INTERLEAVER_SIZE; i < /*n_tailed*/ N_TAILED; i++)
	{
#pragma HLS PIPELINE
		recv_syst[i] = recv_syst1[i];
		int_recv_syst[i] = recv_syst2[i];
	}

#define N_ITERS 1

	// do the iterative decoding
	for (i = 0; i < /*n_iters*/ N_ITERS; i++)
	{
	//	map_decoder(recv_syst1, recv_parity1, Le21, Le12, interleaver_size);
		log_decoder(recv_syst, recv_parity1, Le21, Le12, interleaver_size);
			
		internal_interleaver(Le12, Le12_int, interleaver_size);

		//	memset(Le12_int + interleaver_size, 0, N_TAIL * sizeof(float));
		for (j = interleaver_size/*INTERLEAVER_SIZE*/; j < interleaver_size + N_TAIL /*INTERLEAVER_SIZE + N_TAIL*/; j++)
			Le12_int[j] = 0.0;
	//	map_decoder(recv_syst2, recv_parity2, Le12_int, Le21_int, interleaver_size);
		log_decoder(int_recv_syst, recv_parity2, Le12_int, Le21_int, interleaver_size);

		internal_deinterleaver(Le21_int, Le21, interleaver_size);
		//	memset(Le21 + interleaver_size, 0, N_TAIL * sizeof(float));
		for (j = interleaver_size/*INTERLEAVER_SIZE*/; j < interleaver_size + N_TAIL /*INTERLEAVER_SIZE + N_TAIL*/; j++)
			Le21_int[j] = 0.0;
	}

	for (i = 0; i < /*interleaver_size*/ INTERLEAVER_SIZE; i++)
	{
#pragma HLS PIPELINE
		L[i] = recv_syst[i] + Le21[i] + Le12[i];
	//	std::cout << recv_syst1[i] << "\t" << Le21[i] << "\t" << Le12[i] << std::endl;
		decoded_bits_i[i] = (L[i] > 0.0) ? 1 : -1;
	//	decoded_bits_i[i] = (L[i] >= 0.0) ? 1 : 0;
	}
}

void log_decoder(float recv_syst[N_UNCODED + N_TAIL],
		float recv_parity[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		float apriori[BLOCK_SIZE + N_TAIL],
		float extrinsic[BLOCK_SIZE + N_TAIL],
		int interleaver_size)
{
#pragma HLS ARRAY_PARTITION variable=g_state_trans cyclic factor=2 dim=1
//#pragma HLS DATAFLOW

	float nom, den, temp0, temp1, exp_temp0, exp_temp1, rp;

#pragma HLS DEPENDENCE variable=nom inter false
//#pragma HLS DEPENDENCE variable=den inter false

	int i, j, s0, s1, k, kk, l, s, s_prim, s_prim0, s_prim1;
	int block_length = (interleaver_size + N_TAIL /*INTERLEAVER_SIZE + N_TAIL*/);
	
	float alpha[N_STATES * (BLOCK_SIZE + N_TAIL + 1)];
#pragma HLS ARRAY_PARTITION variable=alpha cyclic factor=2 dim=1
	float beta[N_STATES * (BLOCK_SIZE + N_TAIL + 1)];
	float gamma[N_STATES * 2 * (BLOCK_SIZE + N_TAIL + 1)];
	float denom[BLOCK_SIZE + N_TAIL + 1];

#define BLOCK_LENGTH (INTERLEAVER_SIZE + N_TAIL)
	Lc = 1.0;
//	com_log = max_log;
	for (k = 0; k <= /*block_length*/ BLOCK_LENGTH; k++)
	{
		denom[k] = -LOG_INFINITY;
	}

	// Calculate gamma
	for (k = 1; k <= /*block_length*/ BLOCK_LENGTH; k++)
	{
		kk = k - 1;

		for (s_prim = 0; s_prim < N_STATES; s_prim++)
		{
			/*exp_temp0 = 0.0;
			exp_temp1 = 0.0;*/

			for (j = 0; j < (N_GENS - 1); j++)
			{
#pragma HLS LOOP_FLATTEN
				if(j == 0){
					exp_temp0 = 0.0;
					exp_temp1 = 0.0;
				}

				rp = recv_parity[kk * (N_GENS - 1) + j];
				if (0 == g_output_parity[s_prim * (N_GENS - 1) * 2 + j * 2 + 0])
				{
					exp_temp0 += rp;
				}
				else
				{ 
					exp_temp0 -= rp;
				}
				if (0 == g_output_parity[s_prim * (N_GENS - 1) * 2 + j * 2 + 1])
				{
					exp_temp1 += rp;
				}
				else
				{
					exp_temp1 -= rp; 
				}

				if(j == N_GENS - 2){
					gamma[(2 * s_prim + 0) * (block_length + 1) + k] =  0.5 * ((apriori[kk] + recv_syst[kk]) + exp_temp0);
					gamma[(2 * s_prim + 1) * (block_length + 1) + k] = -0.5 * ((apriori[kk] + recv_syst[kk]) - exp_temp1);
				}
			}

			//gamma[(2 * s_prim + 0) * (block_length + 1) + k] =  0.5 * ((apriori[kk] + recv_syst[kk]) + exp_temp0);
		//	std::cout << gamma[(2 * s_prim + 0) * (block_length + 1) + k] << "\t";
			//gamma[(2 * s_prim + 1) * (block_length + 1) + k] = -0.5 * ((apriori[kk] + recv_syst[kk]) - exp_temp1);
		//	std::cout << gamma[(2 * s_prim + 1) * (block_length + 1) + k] << std::endl;
		}
	}

	// Initiate alpha
	for (int i = 1; i < N_STATES; i++)
	{
		alpha[i * (block_length + 1) + 0] = -LOG_INFINITY;
	}
	alpha[0 * (block_length + 1) + 0] = 0.0;

	// Calculate alpha, going forward through the trellis
	for (k = 1; k <= /*block_length*/ BLOCK_LENGTH; k++)
	{
		for (s = 0; s < N_STATES; s++)
		{
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
			s_prim0 = g_rev_state_trans[s * 2 + 0];
			s_prim1 = g_rev_state_trans[s * 2 + 1];
			temp0 = alpha[s_prim0 * (block_length + 1) + k - 1] + gamma[(2 * s_prim0 + 0) * (block_length + 1) + k];
			temp1 = alpha[s_prim1 * (block_length + 1) + k - 1] + gamma[(2 * s_prim1 + 1) * (block_length + 1) + k];
			//	alpha[s * (block_length + 1) + k] = com_log(temp0, temp1);
			//	denom[k] = com_log(alpha[s * (block_length + 1) + k], denom[k]);
			int temp = max_log(temp0, temp1);
			alpha[s * (block_length + 1) + k] = temp;
			//denom[k] = max_log(alpha[s * (block_length + 1) + k], denom[k]);
			denom[k] = max_log(temp, denom[k]);
		}

		// Normalization of alpha
		for (l = 0; l < N_STATES; l++)
		{
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
			alpha[l * (block_length + 1) + k] -= denom[k];
		}
	}

	// Initiate beta
	for (i = 1; i < N_STATES; i++)
	{
		beta[i * (block_length + 1) + block_length] = -LOG_INFINITY;
	}
	beta[0 * (block_length + 1) + block_length] = 0.0;

	// Calculate beta going backward in the trellis
	for (k = /*block_length*/ BLOCK_LENGTH; k >= 2; k--)
	{
#pragma HLS LOOP_TRIPCOUNT max=6147
		for (s_prim = 0; s_prim < N_STATES; s_prim++)
		{
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
			s0 = g_state_trans[s_prim * 2 + 0];
			s1 = g_state_trans[s_prim * 2 + 1];
			//	beta[s_prim * (block_length + 1) + k - 1] = com_log(beta[s0 * (block_length + 1) + k] + gamma[(2 * s_prim + 0) * (block_length + 1) + k], beta[s1 * (block_length + 1) + k] + gamma[(2 * s_prim + 1) * (block_length + 1) + k]);
			beta[s_prim * (block_length + 1) + k - 1] = max_log(beta[s0 * (block_length + 1) + k] + gamma[(2 * s_prim + 0) * (block_length + 1) + k], beta[s1 * (block_length + 1) + k] + gamma[(2 * s_prim + 1) * (block_length + 1) + k]);
		}
		// Normalization of beta
		for (l = 0; l < N_STATES; l++)
		{
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE array inter false
			beta[l * (block_length + 1) + k - 1] -= denom[k];
		}
	}

	// Calculate extrinsic output for each bit
	for (k = 1; k <= /*block_length*/ BLOCK_LENGTH; k++)
	{
#pragma HLS DEPENDENCE array inter false
		kk = k - 1;
		nom = -LOG_INFINITY;
		den = -LOG_INFINITY;
		for (s_prim = 0; s_prim < N_STATES; s_prim++)
		{
#pragma HLS DEPENDENCE array inter false
#pragma HLS PIPELINE
			s0 = g_state_trans[s_prim * 2 + 0];
			s1 = g_state_trans[s_prim * 2 + 1];
			exp_temp0 = 0.0;
			exp_temp1 = 0.0;
			for (j = 0; j < (N_GENS - 1); j++)
			{
#pragma HLS UNROLL
				rp = recv_parity[kk * (N_GENS - 1) + j];
				int index = s_prim * (N_GENS - 1) * 2 + j * 2;
				if (0 == g_output_parity[index + 0])
				{
					exp_temp0 += rp;
				}
				else
				{
					exp_temp0 -= rp; 
				}
				if (0 == g_output_parity[index + 1])
				{ 
					exp_temp1 += rp;
				}
				else
				{
					exp_temp1 -= rp;
				}
			}
			nom = max_log(nom, alpha[s_prim * (block_length + 1) + kk] + 0.5 * exp_temp0 + beta[s0 * (block_length + 1) + k]);
			den = max_log(den, alpha[s_prim * (block_length + 1) + kk] + 0.5 * exp_temp1 + beta[s1 * (block_length + 1) + k]);
		}
		extrinsic[kk] = nom - den;
	//	std::cout << nom << "\t" << den << std::endl;
	//	std::cout << extrinsic[kk] << std::endl;
	}
}
