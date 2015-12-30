
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#include <iostream>

#include "GeneralFunc.h"
//#include "check.h"

#include "TransformPrecoder.h"


LTE_PHY_PARAMS lte_phy_params;

void test_decoder(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Transform Decoder starts" << std::endl;
	
//	ReadInputFromFiles(lte_phy_params->trans_decoder_in, lte_phy_params->trans_decoder_in_buf_sz, "testTransformPrecoderOutputReal", "testTransformPrecoderOutputImag");
	GeneRandomInput(lte_phy_params->trans_encoder_in, lte_phy_params->trans_encoder_in_buf_sz, "TransformDecoderInputReal", "TransformDecoderInputImag");

	TransformDecoding(lte_phy_params->trans_decoder_in, lte_phy_params->trans_decoder_out, lte_phy_params->N_rx_ant, lte_phy_params->N_dft_sz);

	WriteOutputToFiles(lte_phy_params->trans_decoder_out, lte_phy_params->trans_decoder_out_buf_sz, "testTransformDecoderOutputReal", "testTransformDecoderOutputImag");
	
	std::cout << "Transform Decoder ends" << std::endl;

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

	test_decoder(&lte_phy_params);

	return 0;
}
