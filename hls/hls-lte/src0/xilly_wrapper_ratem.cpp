
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lte_phy.h"
#include "RateMatcher.h"
#include "GeneralFunc.h"

LTE_PHY_PARAMS lte_phy_params;

void rx_rate_matching(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Rx RateMatching starts" << std::endl;

//	ReadInputFromFiles(lte_phy_params->rdm_in, lte_phy_params->rdm_in_buf_sz, "testTxRateMatchOutput");
	GeneRandomInput(lte_phy_params->rdm_in, lte_phy_params->rdm_in_buf_sz, "RxRateMatchInput");

	RxRateMatching(lte_phy_params->rdm_in, lte_phy_params->rdm_out, lte_phy_params->rdm_hard, lte_phy_params->rdm_out_buf_sz);

	WriteOutputToFiles(lte_phy_params->rdm_out, lte_phy_params->rdm_out_buf_sz, "testRxRateMatchOutput");

	std::cout << "Rx RateMatching ends" << std::endl;
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

	rx_rate_matching(&lte_phy_params);

	return 0;
}


