
#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <iostream>
#include <string.h>

#include "GeneralFunc.h"
#include "check.h"

#include "Modulation.h"

//#define Mod

LTE_PHY_PARAMS lte_phy_params;

void test_demod(LTE_PHY_PARAMS *lte_phy_params, int mod_type)
{
		
	std::cout << "Demodulation starts" << std::endl;

	float awgn_sigma = 0.193649; //this value is for the standard input  see "AWGNSigma"
	
//	ReadInputFromFiles(lte_phy_params->demod_in, lte_phy_params->demod_in_buf_sz, "testModulationRandomOutputReal", "testModulationRandomOutputImag");
	GeneRandomInput(lte_phy_params->demod_in, lte_phy_params->demod_in_buf_sz, "testModulationRandomOutputReal", "testModulationRandomOutputImag");

	Demodulating(lte_phy_params->demod_in, lte_phy_params->demod_LLR, lte_phy_params->demod_in_buf_sz, mod_type, awgn_sigma);

	for (int i = 0; i < lte_phy_params->demod_out_buf_sz; i++)
	{
		if (lte_phy_params->demod_LLR[i] > 0)
			lte_phy_params->demod_HD[i] = 1;
		else
			lte_phy_params->demod_HD[i] = 0;
	}
	
	WriteOutputToFiles(lte_phy_params->demod_HD, lte_phy_params->demod_out_buf_sz, "testDemodulationOutput");

	std::cout << "Demodulation ends" << std::endl;
}

int main(int argc, char *argv[])
{
	int fs_id;
	int n_tx_ant, n_rx_ant;
	int mod_type;

	char tx_in_fname[50];
	char rx_out_fname[50];
	int err_n;
	
	if (argc != 5 && argc != 2)
	{
		lte_phy_usage_info(argv[0]);
		
		return 1;
	}

	if (argc == 5)
	{
		fs_id = atoi(argv[1]);
		mod_type = atoi(argv[2]);
		n_tx_ant = atoi(argv[3]);
		n_rx_ant = atoi(argv[4]);

		lte_phy_init(&lte_phy_params, fs_id, mod_type, n_tx_ant, n_rx_ant);
	}
	else
	{
		fs_id = atoi(argv[1]);

		lte_phy_init(&lte_phy_params, fs_id);
	}

//	test_demod(&lte_phy_params, mod_type);
	test_demod(&lte_phy_params, 2);

	return 0;
}

