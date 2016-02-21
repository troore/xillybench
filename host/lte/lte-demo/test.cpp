
#include <stdio.h>

#include "lte_phy.h"
#include "parse_args.h"
#include "general_func.h"

#include "simple_ul_rx.h"

extern "C" {
#include "timer.h"
#include "rapl_power.h"
}

LTE_PHY_PARAMS lte_phy_params;

void test_equalizer_chain_seq(LTE_PHY_PARAMS *lte_phy_params)
{
	double tstart, tend, ttime;
	
	GeneRandomInput((lte_phy_params + 0)->ofdemod_in, (lte_phy_params + 0)->ofdemod_in_buf_sz, "./testsuite/SubCarrierMapInputReal", "./testsuite/SubCarrierMapInputImag");

	tstart = dtime();

	rapl_power_start();
	
	equalizer_chain_seq(&(lte_phy_params[0]));

	rapl_power_stop();

	tend = dtime();
	ttime = tend - tstart;
	printf("time_total %.3f s\n", ttime / 1000.0);

	WriteOutputToFiles((lte_phy_params + 0)->eq_out, (lte_phy_params + 0)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal", "./testsuite/testLSCELSEqOutputImag");
	WriteOutputToFiles((lte_phy_params + 0)->eq_out, (lte_phy_params + 0)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal_co", "./testsuite/testLSCELSEqOutputImag_co");
}

int main(int argc, char *argv[])
{
	parse_args_and_init_lte_phy(argc, argv, &lte_phy_params);
	
	test_equalizer_chain_seq(&lte_phy_params);
	
	return 0;
}

