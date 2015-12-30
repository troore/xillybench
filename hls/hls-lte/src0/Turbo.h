
#ifndef __TURBO_H_
#define __TURBO_H_

#include "lte_phy.h"

#define TURBO_INT_K_TABLE_SIZE 188


int reverse_int(int length, int in);
int calc_state_transition(int instate, int input, int parity[]);
void set_generator_polynomials(int gens[], int n_gens, int constraint_length);

void turbo_encoding(int piSeq[N_TURBO_IN_MAX], int pcSeq[N_TURBO_OUT_MAX], int input_data_length);

void encode_block(int input_bits[BLOCK_SIZE],
				  int interleaved_input_bits[BLOCK_SIZE],
				  int parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)], 
				  int tail1[N_TAIL],
				  int parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
				  int tail2[N_TAIL], 
				  int blk_len);

void constituent_encoder(int *input, int input_len, int *tail, int *parity);

void turbo_decoding(float pInpData[N_TURBO_OUT_MAX], int pOutBits[N_TURBO_IN_MAX], int out_data_length, int n_iters);

void decode_block(float recv_syst1[N_UNCODED + N_TAIL], 
		float recv_syst2[N_UNCODED + N_TAIL],
		float recv_parity1[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		float recv_parity2[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		int decoded_bits_i[BLOCK_SIZE], 
		int interleaver_size,
		int n_iters);

void log_decoder(float recv_syst[N_UNCODED + N_TAIL],
		float recv_parity[(N_UNCODED + N_TAIL) * (N_GENS - 1)],
		float apriori[BLOCK_SIZE + N_TAIL],
		float extrinsic[BLOCK_SIZE + N_TAIL],
		int interleaver_size);

float max_log(float a, float b);
	
//template <typename T> void internal_interleaver(T in[BLOCK_SIZE], T out[BLOCK_SIZE], int interleaver_size);
//template <typename T> void internal_deinterleaver(T in[BLOCK_SIZE], T out[BLOCK_SIZE], int deinterleaver_size);
void internal_interleaver(int in[BLOCK_SIZE], int out[BLOCK_SIZE], int interleaver_size);
void internal_deinterleaver(int in[BLOCK_SIZE], int out[BLOCK_SIZE], int deinterleaver_size);
void internal_interleaver(float in[BLOCK_SIZE], float out[BLOCK_SIZE], int interleaver_size);
void internal_deinterleaver(float in[BLOCK_SIZE], float out[BLOCK_SIZE], int deinterleaver_size);

extern int InpBlockShift;
extern int OutBlockShift;
extern int NumBlock;
extern int DataLength;
extern int LastBlockLength;
	
extern int g_gens[N_GENS];

extern int g_gen_pols[N_GENS];
extern int g_rev_gen_pols[N_GENS];
extern int g_state_trans[N_STATES * 2];
extern int g_rev_state_trans[N_STATES * 2];
extern int g_output_parity[N_STATES * (N_GENS - 1) * 2];
extern int g_rev_output_parity[N_STATES * (N_GENS - 1) * 2];

extern float Lc;
	
#endif
