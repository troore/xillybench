
#include <stdio.h>
#include "lte_phy.h"
#include "parse_args.h"
#include "GeneralFunc.h"

#include "SimpleULRx.h"
#include "timer/timer.h"

#define NUM_THREADS_SIMPLE_RX 3

LTE_PHY_PARAMS lte_phy_params[NUM_THREADS_SIMPLE_RX];
int num_frames;

void test_equalizer_chain_seq(LTE_PHY_PARAMS *lte_phy_params)
{
	double tstart, tend, ttime;
	
	GeneRandomInput((lte_phy_params + 0)->ofdemod_in, (lte_phy_params + 0)->ofdemod_in_buf_sz, "./testsuite/SubCarrierMapInputReal", "./testsuite/SubCarrierMapInputImag");

	tstart = dtime();
	
	equalizer_chain_seq(&(lte_phy_params[0]));

	tend = dtime();
	ttime = tend - tstart;
//	printf("%.3fms\n", ttime);

	WriteOutputToFiles((lte_phy_params + 0)->eq_out, (lte_phy_params + 0)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal", "./testsuite/testLSCELSEqOutputImag");
}

void test_equalizer_chain_pth(LTE_PHY_PARAMS *lte_phy_params)
{
	double tstart, tend, ttime;
	
	GeneRandomInput((lte_phy_params + 0)->ofdemod_in, (lte_phy_params + 0)->ofdemod_in_buf_sz, "./testsuite/SubCarrierMapInputReal", "./testsuite/SubCarrierMapInputImag");
	GeneRandomInput((lte_phy_params + 1)->ofdemod_in, (lte_phy_params + 1)->ofdemod_in_buf_sz, "./testsuite/SubCarrierMapInputReal", "./testsuite/SubCarrierMapInputImag");
	GeneRandomInput((lte_phy_params + 2)->ofdemod_in, (lte_phy_params + 2)->ofdemod_in_buf_sz, "./testsuite/SubCarrierMapInputReal", "./testsuite/SubCarrierMapInputImag");

	tstart = dtime();
	
	equalizer_chain_pth(lte_phy_params);

	tend = dtime();
	ttime = tend - tstart;
	printf("%.3fms\n", ttime);

	WriteOutputToFiles((lte_phy_params + 0)->eq_out, (lte_phy_params + 0)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal", "./testsuite/testLSCELSEqOutputImag");
	WriteOutputToFiles((lte_phy_params + 1)->eq_out, (lte_phy_params + 1)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal", "./testsuite/testLSCELSEqOutputImag");
	WriteOutputToFiles((lte_phy_params + 2)->eq_out, (lte_phy_params + 2)->eq_out_buf_sz, "./testsuite/testLSCELSEqOutputReal", "./testsuite/testLSCELSEqOutputImag");
}

void test(LTE_PHY_PARAMS *lte_phy_params)
{
#ifndef PTH
	test_equalizer_chain_seq(lte_phy_params);
#else
	test_equalizer_chain_pth(lte_phy_params);
#endif

}

int main(int argc, char *argv[])
{
	parse_args_and_init_lte_phy(argc, argv, lte_phy_params, NUM_THREADS_SIMPLE_RX);
//	parse_args_and_init_lte_phy(argc, argv, &(lte_phy_params[1]));
//	parse_args_and_init_lte_phy(argc, argv, &(lte_phy_params[2]));
	
	test(lte_phy_params);
	
	return 0;
}

