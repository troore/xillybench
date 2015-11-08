
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "lte_phy.h"
#include "test.h"
#include "util.h"

static const char *opt_str = "f:t:r:m:n:h";

static const struct option long_opts[] = {
	{ "subframe-index", required_argument, NULL, 'f' },
	{ "num-tx-antennas", required_argument, NULL, 't' },
	{ "num-rx-antennas", required_argument, NULL, 'r' },
	{ "modulation-type", required_argument, NULL, 'm' },
//	{ "num-threads", required_argument, NULL, 'n' },
	{ "num-frames", required_argument, NULL, 'n' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

void parse_args_and_init_lte_phy(int argc, char *argv[], 
		LTE_PHY_PARAMS *lte_phy_params, int n)
{
	int sf_id = 0;
	int n_tx_ant  = 2, n_rx_ant  = 2;
	int mod_type = 2;

	int opt = 0;
	int long_id = 0;
	int is_sf_id_set = 0;
	int is_help = 0;

	// default values
	sf_id = 0;
	n_tx_ant = 2;
	n_rx_ant = 2;
	mod_type = 2;
	num_frames = 1;

	/*
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
	*/
//	opt = getopt(argc, argv, opt_str);
	opt = getopt_long(argc, argv, opt_str, long_opts, &long_id);
	while (opt != -1)
	{
		switch(opt)
		{
		case 'f':
			sf_id = atoi(optarg);
			is_sf_id_set = 1;
			break;
		case 't':
			n_tx_ant  = atoi(optarg);
			//	flag = 1;
			break;
		case 'r':
			n_rx_ant  = atoi(optarg);
			//	flag = 1;
			break;
		case 'm':
			mod_type = atoi(optarg);
			//	flag = 1;
			break;
		case 'n':
	//		num_threads = atoi(optarg);
			num_frames = atoi(optarg);
			//	flag = 1;
			break;
		case 'h':
			is_help = 1;
			break;
		default:
			printf("Unrecognized argument: %c\n", opt);
			break;
		}

		//	opt = getopt(argc, argv, opt_str);
		opt = getopt_long(argc, argv, opt_str, long_opts, &long_id);
	}

	if (is_help)
	{
		lte_phy_usage_info(argv[0]);
	}
	if (!is_sf_id_set)
	{
		printf("You should set the subframe index at least to run!\n");
		exit(0);
	}
	else
	{
		int i;

		for (i = 0; i < n; i++)
			lte_phy_init(&lte_phy_params[i], sf_id, mod_type, n_tx_ant, n_rx_ant);
	}
}
