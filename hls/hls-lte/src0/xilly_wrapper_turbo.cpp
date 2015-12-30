
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "Turbo.h"
#include "GeneralFunc.h"
#include "gauss.h"
#include "lte_phy.h"

//#define TurboEnc

LTE_PHY_PARAMS lte_phy_params;

void test_turbo_decoding(LTE_PHY_PARAMS *lte_phy_params, int n_iters)
{
	std::cout << "Turbo Decoder starts" << std::endl;
	
//	ReadInputFromFiles(lte_phy_params->td_in, lte_phy_params->td_in_buf_sz, "testTurboEncoderOutput");
	GeneRandomInput(lte_phy_params->td_in, lte_phy_params->td_in_buf_sz, "TurboDecoderInput");
	
	for (int i = 0; i < lte_phy_params->td_in_buf_sz; i++)
	{
		lte_phy_params->td_in[i] = (1 - 2 * lte_phy_params->td_in[i]);
	}

	turbo_decoding(lte_phy_params->td_in, lte_phy_params->td_out, lte_phy_params->td_out_buf_sz, n_iters);

	for (int i = 0; i < lte_phy_params->td_out_buf_sz; i++)
	{
		lte_phy_params->td_out[i] = (1 - lte_phy_params->td_out[i]) / 2;
		//	printf("%d", lte_phy_params->td_out[i]);
	}
//	printf("\n");
	
	WriteOutputToFiles(lte_phy_params->td_out, lte_phy_params->td_out_buf_sz, "testTurboDecoderOutput");
	
	std::cout << "Turbo Decoder ends" << std::endl;
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

	int n_iters = 1;
	
	test_turbo_decoding(&lte_phy_params, n_iters);

	return 0;
}

