
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "lte_phy.h"

#include "equalizer.h"
#include "res_mapper.h"
#include "OFDM.h"
#include "dmrs.h"

#include "test.h"

extern "C" {
#include "timer.h"
#include "rapl_power.h"
}

void equalizer_chain_seq(LTE_PHY_PARAMS *lte_phy_params)
{
	int i;

	double tstart, tend, ttime;

	int h;
	
	geneDMRS(lte_phy_params->DMRS, lte_phy_params->N_tx_ant, lte_phy_params->N_dft_sz);
	
//	SubCarrierMapping(lte_phy_params, lte_phy_params->resm_in, lte_phy_params->resm_out);
//	ofmodulating(lte_phy_params, lte_phy_params->resm_out, lte_phy_params->ofmod_out);
	ttime = 0.0;
//	rapl_power_start();
	tstart = dtime();
	ofdemodulating(lte_phy_params, lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out);
	tend = dtime();
	ttime = ttime + (tend - tstart);
	printf("time_ofdm %.3f s\n", ttime / 1000.0);

	for (h = 0; h < 100; h++) {
//	tstart = dtime();

	SubCarrierDemapping(lte_phy_params, lte_phy_params->ofdemod_out, lte_phy_params->resdm_out);
	//	tend = dtime();
	//	ttime = ttime + (tend - tstart);
	//	tstart = dtime();
	Equalizing(lte_phy_params, lte_phy_params->resdm_out, lte_phy_params->eq_out);
	//	tend = dtime();
	//	ttime = ttime + (tend - tstart);
//	rapl_power_stop();
	//	printf("%.3fms\n", ttime);
	}
}

