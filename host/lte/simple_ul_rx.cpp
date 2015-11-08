
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <pthread.h>

#include "lte_phy.h"

#include "Equalizer.h"
#include "ResMapper.h"
#include "OFDM.h"
#include "refs/dmrs.h"
#include "timer/timer.h"

#include "test.h"
#include "util.h"

pthread_mutex_t thread_counter_mutex;
pthread_cond_t thread_counter_cv;
int thread_counter;

void sync_threads(int num_threads)
{
	pthread_mutex_lock(&thread_counter_mutex);
	thread_counter++;

	if (thread_counter < num_threads)
		pthread_cond_wait(&thread_counter_cv, &thread_counter_mutex);

	pthread_mutex_unlock(&thread_counter_mutex);

	if (thread_counter == num_threads) {
		thread_counter = 0;
		pthread_cond_broadcast(&thread_counter_cv);
	}
}

void *thread_fun_eq(void *thread_params)
{
	LTE_PHY_PARAMS *lte_phy_params = (LTE_PHY_PARAMS *)thread_params;
	
	int i, j;

	for (i = 2; i < num_frames; i++) {
		Equalizing(lte_phy_params, lte_phy_params->eq_in, lte_phy_params->eq_out);

		sync_threads(3);

		// In fact, there should be input to the thread_fun_ofdemod,
		// but we use old data instead. It doesn't affect performance and power.
	}
}


void *thread_fun_resdm(void *thread_params)
{
	LTE_PHY_PARAMS *lte_phy_params = (LTE_PHY_PARAMS *)thread_params;
	
	int i, j;

	for (i = 2; i < num_frames; i++) {
		SubCarrierDemapping(lte_phy_params, lte_phy_params->resdm_in, lte_phy_params->resdm_out);

		sync_threads(3);

		if ((i + 1) < num_frames) {
			for (j = 0; j < lte_phy_params->resdm_out_buf_sz; j++)
				lte_phy_params->eq_in[j] = lte_phy_params->resdm_out[j];
		}
	}
}


void *thread_fun_ofdemod(void *thread_params)
{
	LTE_PHY_PARAMS *lte_phy_params = (LTE_PHY_PARAMS *)thread_params;
	
	int i, j;

	for (i = 2; i < num_frames; i++) {
		ofdemodulating(lte_phy_params, lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out);

		sync_threads(3);

		if ((i + 1) < num_frames) {
			for (j = 0; j < lte_phy_params->ofdemod_out_buf_sz; j++)
				lte_phy_params->resdm_in[j] = lte_phy_params->ofdemod_out[j];
		}
	}
}


void equalizer_chain_seq(LTE_PHY_PARAMS *lte_phy_params)
{
	int i;

//	double tstart, tend, ttime;
	
	geneDMRS(lte_phy_params->DMRS, lte_phy_params->N_tx_ant, lte_phy_params->N_dft_sz);
	
//	SubCarrierMapping(lte_phy_params, lte_phy_params->resm_in, lte_phy_params->resm_out);
//	ofmodulating(lte_phy_params, lte_phy_params->resm_out, lte_phy_params->ofmod_out);
	for (i = 0; i < num_frames; i++) {
//		tstart = dtime();
		ofdemodulating(lte_phy_params, lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out);
//		tend = dtime();
//		printf("%.3fms\n", tend - tstart);
		SubCarrierDemapping(lte_phy_params, lte_phy_params->ofdemod_out, lte_phy_params->resdm_out);
//		tstart = dtime();
//		printf("%.3fms\n", tstart - tend);
		Equalizing(lte_phy_params, lte_phy_params->resdm_out, lte_phy_params->eq_out);
//		tend = dtime();
//		printf("%.3fms\n", tend - tstart);
	}
}

void equalizer_chain_pth(LTE_PHY_PARAMS *lte_phy_params)
{
	pthread_t thread_ofdemod;
	pthread_t thread_resdm;
	pthread_t thread_eq;
	
	geneDMRS((lte_phy_params + 0)->DMRS, (lte_phy_params + 0)->N_tx_ant, (lte_phy_params + 0)->N_dft_sz);
	geneDMRS((lte_phy_params + 1)->DMRS, (lte_phy_params + 1)->N_tx_ant, (lte_phy_params + 1)->N_dft_sz);
	geneDMRS((lte_phy_params + 2)->DMRS, (lte_phy_params + 2)->N_tx_ant, (lte_phy_params + 2)->N_dft_sz);
	
	ofdemodulating(lte_phy_params + 0, (lte_phy_params + 0)->ofdemod_in, (lte_phy_params + 0)->ofdemod_out);
	SubCarrierDemapping(lte_phy_params + 0, (lte_phy_params + 0)->ofdemod_out, (lte_phy_params + 0)->resdm_out);

	ofdemodulating(lte_phy_params + 1, (lte_phy_params + 1)->ofdemod_in, (lte_phy_params + 1)->ofdemod_out);

	pthread_create(&thread_eq, NULL, &thread_fun_eq, lte_phy_params);
	pthread_create(&thread_resdm, NULL, &thread_fun_resdm, lte_phy_params);
	pthread_create(&thread_ofdemod, NULL, &thread_fun_ofdemod, lte_phy_params);

	Equalizing(lte_phy_params + 1, (lte_phy_params + 1)->resdm_out, (lte_phy_params + 1)->eq_out);

	SubCarrierDemapping(lte_phy_params + 2, (lte_phy_params + 2)->ofdemod_out, (lte_phy_params + 2)->resdm_out);
	Equalizing(lte_phy_params + 2, (lte_phy_params + 2)->resdm_out, (lte_phy_params + 2)->eq_out);

	pthread_join(thread_eq, NULL);
	pthread_join(thread_resdm, NULL);
	pthread_join(thread_ofdemod, NULL);
}

