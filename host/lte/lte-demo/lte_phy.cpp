#include <stdio.h>
#include "lte_phy.h"

// Turbo Internal Interleaver from 3GPP TS 36.212 v10.0.0 table 5.1.3-3

int TURBO_INT_K_TABLE[TURBO_INT_K_TABLE_SIZE] = {  40,  48,  56,  64,  72,  80,  88,  96, 104, 112,
												   120, 128, 136, 144, 152, 160, 168, 176, 184, 192,
												   200, 208, 216, 224, 232, 240, 248, 256, 264, 272,
												   280, 288, 296, 304, 312, 320, 328, 336, 344, 352,
												   360, 368, 376, 384, 392, 400, 408, 416, 424, 432,
												   440, 448, 456, 464, 472, 480, 488, 496, 504, 512,
												   528, 544, 560, 576, 592, 608, 624, 640, 656, 672,
												   688, 704, 720, 736, 752, 768, 784, 800, 816, 832,
												   848, 864, 880, 896, 912, 928, 944, 960, 976, 992,
												   1008,1024,1056,1088,1120,1152,1184,1216,1248,1280,
												   1312,1344,1376,1408,1440,1472,1504,1536,1568,1600,
												   1632,1664,1696,1728,1760,1792,1824,1856,1888,1920,
												   1952,1984,2016,2048,2112,2176,2240,2304,2368,2432,
												   2496,2560,2624,2688,2752,2816,2880,2944,3008,3072,
												   3136,3200,3264,3328,3392,3456,3520,3584,3648,3712,
												   3776,3840,3904,3968,4032,4096,4160,4224,4288,4352,
												   4416,4480,4544,4608,4672,4736,4800,4864,4928,4992,
												   5056,5120,5184,5248,5312,5376,5440,5504,5568,5632,
												   5696,5760,5824,5888,5952,6016,6080,6144};

int TURBO_INT_F1_TABLE[TURBO_INT_K_TABLE_SIZE] = {  3,  7, 19,  7,  7, 11,  5, 11,  7, 41,103, 15,  9,
													17,  9, 21,101, 21, 57, 23, 13, 27, 11, 27, 85, 29,
													33, 15, 17, 33,103, 19, 19, 37, 19, 21, 21,115,193,
													21,133, 81, 45, 23,243,151,155, 25, 51, 47, 91, 29,
													29,247, 29, 89, 91,157, 55, 31, 17, 35,227, 65, 19,
													37, 41, 39,185, 43, 21,155, 79,139, 23,217, 25, 17,
													127, 25,239, 17,137,215, 29, 15,147, 29, 59, 65, 55,
													31, 17,171, 67, 35, 19, 39, 19,199, 21,211, 21, 43,
													149, 45, 49, 71, 13, 17, 25,183, 55,127, 27, 29, 29,
													57, 45, 31, 59,185,113, 31, 17,171,209,253,367,265,
													181, 39, 27,127,143, 43, 29, 45,157, 47, 13,111,443,
													51, 51,451,257, 57,313,271,179,331,363,375,127, 31,
													33, 43, 33,477, 35,233,357,337, 37, 71, 71, 37, 39,
													127, 39, 39, 31,113, 41,251, 43, 21, 43, 45, 45,161,
													89,323, 47, 23, 47,263};

int TURBO_INT_F2_TABLE[TURBO_INT_K_TABLE_SIZE] = { 10, 12, 42, 16, 18, 20, 22, 24, 26, 84, 90, 32, 34,
												   108, 38,120, 84, 44, 46, 48, 50, 52, 36, 56, 58, 60,
												   62, 32,198, 68,210, 36, 74, 76, 78,120, 82, 84, 86,
												   44, 90, 46, 94, 48, 98, 40,102, 52,106, 72,110,168,
												   114, 58,118,180,122, 62, 84, 64, 66, 68,420, 96, 74,
												   76,234, 80, 82,252, 86, 44,120, 92, 94, 48, 98, 80,
												   102, 52,106, 48,110,112,114, 58,118, 60,122,124, 84,
												   64, 66,204,140, 72, 74, 76, 78,240, 82,252, 86, 88,
												   60, 92,846, 48, 28, 80,102,104,954, 96,110,112,114,
												   116,354,120,610,124,420, 64, 66,136,420,216,444,456,
												   468, 80,164,504,172, 88,300, 92,188, 96, 28,240,204,
												   104,212,192,220,336,228,232,236,120,244,248,168, 64,
												   130,264,134,408,138,280,142,480,146,444,120,152,462,
												   234,158, 80, 96,902,166,336,170, 86,174,176,178,120,
												   182,184,186, 94,190,480};



void lte_phy_usage_info(char *exec)
{
	printf("Oops! Your should type 4 arguments or just 1 argument for this application.\n");
	printf("=================\n");
	printf("4 arguments case:\n");
	printf("\t%s SampRateID ModType TxAntennaNum RxAntennaNum\n\n", exec);
	printf("\tSampRateID\n");
	printf("\t\tIndex of input streaming bits sampling rate, chosen from 0, 1, 2, 3, and 4.\n");
	printf("\n\
\t\t+-------+--------------+------------+----+----+---+------+\n \
\t\t|ID\t|SampRate (MHz)|SampsPerSymb|NDFT|NFFT|CP0|CPElse|\n	\
\t+-------+--------------+------------+----+----+---+------+\n \
\t\t|0\t|     1.92     |     128    |75  |128 | 9 |  9   |\n \
\t\t+-------+--------------+------------+----+----+---+------+\n	\
\t|1\t|     3.84     |     256    |150 |256 | 9 |  9   |\n \
\t\t+-------+--------------+------------+----+----+---+------+\n	\
\t|2\t|     7.68     |     512    |300 |512 | 9 |  9   |\n \
\t\t+-------+--------------+------------+----+----+---+------+\n	\
\t|3\t|     15.36    |     1024   |600 |1024| 9 |  9   |\n \
\t\t+-------+--------------+------------+----+----+---+------+\n	\
\t|4\t|     30.72    |     2048   |1200|2048| 9 |  9   |\n \
\t\t+-------+--------------+------------+----+----+---+------+\n	\
\n");
	printf("\tModType\n");
	printf("\t\tModulation type, chosen from 0, 1, 2 and 3.\n");
	printf("\n\
\t\t+---------------+-------------+\n\
\t\t|    ModType\t|Bits(Complex)|\n\
\t\t+-------+-------+-------------+\n\
\t\t|0\t|BPSK\t|    1(1)     |\n\
\t\t+-------+-------+-------------+\n\
\t\t|1\t|QPSK\t|    2(1)     |\n\
\t\t+-------+-------+-------------+\n\
\t\t|2\t|16QAM\t|    4(1)     |\n\
\t\t+-------+-------+-------------+\n\
\t\t|3\t|64QAM\t|    6(1)     |\n\
\t\t+-------+-------+-------------+\n\
\n");
	printf("\tTxAntennaNum\n");
	printf("\t\tNumber of Tx antennas, chosen within [1, 8].\n");
	printf("\n\tRxAntennaNum\n");
	printf("\t\tNumber of Rx antennas, chosen within [1, 8].\n\n");
	printf("1 argument case:\n");
	printf("\t%s SampRateID\n", exec);
	printf("\n\tThis case is used for testing and statistic purpose. You only need to give the sampling rate parameter to set the NDFT, NFFT, etc. Other parameters are set automatically to default values, i.e., modulation type is 16QAM, and numbers of Tx and Rx antennas are both 2.\n");
}

void lte_phy_iolen(LTE_PHY_PARAMS *lte_phy_params)
{
	// Compute data length for each module
	// Turbo
	lte_phy_params->te_in_buf_sz = lte_phy_params->data_len_per_subfr;
	lte_phy_params->te_out_buf_sz = RATE * (lte_phy_params->te_in_buf_sz + 4 * ((lte_phy_params->te_in_buf_sz + BLOCK_SIZE - 1) / BLOCK_SIZE));
	lte_phy_params->td_in_buf_sz = lte_phy_params->te_out_buf_sz;
	lte_phy_params->td_out_buf_sz = lte_phy_params->te_in_buf_sz;

	// Rate matching
	lte_phy_params->rm_in_buf_sz = lte_phy_params->te_out_buf_sz;
	lte_phy_params->rm_out_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * (lte_phy_params->N_symb_per_subfr - 2) * lte_phy_params->N_bits_per_samp;
	lte_phy_params->rdm_in_buf_sz = lte_phy_params->rm_out_buf_sz;
	lte_phy_params->rdm_out_buf_sz = lte_phy_params->rm_in_buf_sz;

	// Scrambling
	lte_phy_params->scramb_in_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * (lte_phy_params->N_symb_per_subfr - 2) * lte_phy_params->N_bits_per_samp;
	lte_phy_params->scramb_out_buf_sz = lte_phy_params->scramb_in_buf_sz;
	lte_phy_params->descramb_in_buf_sz = lte_phy_params->scramb_in_buf_sz;
	lte_phy_params->descramb_out_buf_sz = lte_phy_params->scramb_in_buf_sz;

	// Modulation
	lte_phy_params->mod_in_buf_sz = lte_phy_params->scramb_out_buf_sz;
	lte_phy_params->mod_out_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * (lte_phy_params->N_symb_per_subfr - 2);
	lte_phy_params->demod_in_buf_sz = lte_phy_params->mod_out_buf_sz;
	lte_phy_params->demod_out_buf_sz = lte_phy_params->mod_in_buf_sz;

	// Transform precoding
	lte_phy_params->trans_encoder_in_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * (LTE_PHY_N_SYMB_PER_SUBFR - 2);
	lte_phy_params->trans_encoder_out_buf_sz = lte_phy_params->trans_encoder_in_buf_sz;
	lte_phy_params->trans_decoder_in_buf_sz = lte_phy_params->trans_encoder_in_buf_sz;
	lte_phy_params->trans_decoder_out_buf_sz = lte_phy_params->trans_encoder_in_buf_sz;

	// Resource mapping
	lte_phy_params->resm_in_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * (lte_phy_params->N_symb_per_subfr - 2);
	lte_phy_params->resm_out_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_fft_sz * lte_phy_params->N_symb_per_subfr;
	lte_phy_params->resdm_in_buf_sz = lte_phy_params->resm_out_buf_sz;
	lte_phy_params->resdm_out_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_dft_sz * lte_phy_params->N_symb_per_subfr;

	// Equalizing
	lte_phy_params->eq_in_buf_sz = lte_phy_params->N_rx_ant * LTE_PHY_N_SYMB_PER_SUBFR * lte_phy_params->N_dft_sz;
	lte_phy_params->eq_out_buf_sz = lte_phy_params->N_rx_ant * (lte_phy_params->N_symb_per_subfr - 2) * lte_phy_params->N_dft_sz;

	// OFDM
	lte_phy_params->ofmod_in_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_fft_sz * lte_phy_params->N_symb_per_subfr;
	lte_phy_params->ofmod_out_buf_sz = lte_phy_params->N_tx_ant * (lte_phy_params->N_fft_sz + lte_phy_params->N_samps_cp_l_0) * lte_phy_params->N_symb_per_subfr;
	lte_phy_params->ofdemod_in_buf_sz = lte_phy_params->ofmod_out_buf_sz;
	lte_phy_params->ofdemod_out_buf_sz = lte_phy_params->ofmod_in_buf_sz;
}

/*
void lte_phy_init(LTE_PHY_PARAMS *lte_phy_params, int fs_id, int mod_type, int n_tx_ant, int n_rx_ant)
{
	switch(fs_id)
	{
	case LTE_PHY_FS_1_92MHZ:
		lte_phy_params->fs = 1920000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_1_92MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_1_92MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_1_92MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_1_92MHZ;
		lte_phy_params->data_len_per_subfr = 2368;
		break;
	case LTE_PHY_FS_3_84MHZ:
		lte_phy_params->fs = 3840000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_3_84MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_3_84MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_3_84MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_3_84MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_3_84MHZ;
		lte_phy_params->data_len_per_subfr = 4736;
		break;
	case LTE_PHY_FS_7_68MHZ:
		lte_phy_params->fs = 7680000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_7_68MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_7_68MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_7_68MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_7_68MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_7_68MHZ;
		lte_phy_params->data_len_per_subfr = 6144;
		break;
	case LTE_PHY_FS_15_36MHZ:
		lte_phy_params->fs = 15360000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_15_36MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_15_36MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_15_36MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_15_36MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_15_36MHZ;
		lte_phy_params->data_len_per_subfr = 18432;
		break;
	case LTE_PHY_FS_30_72MHZ:
		lte_phy_params->fs = 30720000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_30_72MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_30_72MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_30_72MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_30_72MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_30_72MHZ;
		lte_phy_params->data_len_per_subfr = 36864;
		break;
	case LTE_PHY_FS_TEST:
		lte_phy_params->fs = 0;
		lte_phy_params->N_samps_per_symb  = 0;
		lte_phy_params->N_dft_sz = 0;
		lte_phy_params->N_fft_sz = 0;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_ELSE_30_72MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_30_72MHZ;
		lte_phy_params->data_len_per_subfr = 6144;
		break;
	default:
		break;
	}

	lte_phy_params->mod_type = mod_type;
	
	switch(mod_type)
	{
	case LTE_BPSK:
		lte_phy_params->N_bits_per_samp = BPSK_BITS_PER_SAMP;
		break;
	case LTE_QPSK:
		lte_phy_params->N_bits_per_samp = QPSK_BITS_PER_SAMP;
		break;
	case LTE_QAM16:
		lte_phy_params->N_bits_per_samp = QAM16_BITS_PER_SAMP;
		break;
	case LTE_QAM64:
		lte_phy_params->N_bits_per_samp = QAM64_BITS_PER_SAMP;
		break;
	default:
		break;
	}

	lte_phy_params->N_symb_per_subfr = LTE_PHY_N_SYMB_PER_SUBFR;
	
	// Currently, number of TX antennas and RX antennas is same
	lte_phy_params->N_tx_ant = n_tx_ant;
	lte_phy_params->N_rx_ant = n_rx_ant;

	lte_phy_params->dmrs_symb_pos[0] = 3;
	lte_phy_params->dmrs_symb_pos[1] = 10;

	lte_phy_params->sc_loc = 1;

	lte_phy_iolen(lte_phy_params);
}
*/

void lte_phy_init(LTE_PHY_PARAMS *lte_phy_params, int sf_id, int mod_type, int n_tx_ant, int n_rx_ant)
{
	int num_blks;
	int subfr_len_limit;
	int i;
	
	switch(sf_id)
	{
	case LTE_PHY_FS_1_92MHZ:
		lte_phy_params->fs = 1920000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_1_92MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_1_92MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_1_92MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_1_92MHZ;
		//	lte_phy_params->data_len_per_subfr = 2368;
		break;
	case LTE_PHY_FS_3_84MHZ:
		lte_phy_params->fs = 3840000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_3_84MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_3_84MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_3_84MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_3_84MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_3_84MHZ;
		break;
	case LTE_PHY_FS_7_68MHZ:
		lte_phy_params->fs = 7680000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_7_68MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_7_68MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_7_68MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_7_68MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_7_68MHZ;
		break;
	case LTE_PHY_FS_15_36MHZ:
		lte_phy_params->fs = 15360000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_15_36MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_15_36MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_15_36MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_15_36MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_15_36MHZ;
		break;
	case LTE_PHY_FS_30_72MHZ:
		lte_phy_params->fs = 30720000;
		lte_phy_params->N_samps_per_symb  = LTE_PHY_N_SAMPS_PER_SYMB_30_72MHZ;
		lte_phy_params->N_dft_sz = LTE_PHY_DFT_SIZE_30_72MHZ;
		lte_phy_params->N_fft_sz = LTE_PHY_FFT_SIZE_30_72MHZ;
		lte_phy_params->N_samps_cp_l_0    = LTE_PHY_N_SAMPS_CP_L_0_30_72MHZ;
		lte_phy_params->N_samps_cp_l_else = LTE_PHY_N_SAMPS_CP_L_ELSE_30_72MHZ;
		break;
	default:
		break;
	}

	lte_phy_params->mod_type = mod_type;

	switch(mod_type)
	{
	case LTE_BPSK:
		lte_phy_params->N_bits_per_samp = BPSK_BITS_PER_SAMP;
		break;
	case LTE_QPSK:
		lte_phy_params->N_bits_per_samp = QPSK_BITS_PER_SAMP;
		break;
	case LTE_QAM16:
		lte_phy_params->N_bits_per_samp = QAM16_BITS_PER_SAMP;
		break;
	case LTE_QAM64:
		lte_phy_params->N_bits_per_samp = QAM64_BITS_PER_SAMP;
		break;
	default:
		break;
	}
//	lte_phy_params->N_bits_per_samp = QAM16_BITS_PER_SAMP;

	lte_phy_params->N_symb_per_subfr = LTE_PHY_N_SYMB_PER_SUBFR;

	lte_phy_params->N_tx_ant = n_tx_ant;
	lte_phy_params->N_rx_ant = n_rx_ant;

	lte_phy_params->dmrs_symb_pos[0] = 3;
	lte_phy_params->dmrs_symb_pos[1] = 10;

	lte_phy_params->sc_loc = 1;
	
	//  2 = number of tx antennas; 4 = log(16), 16QAM.
	subfr_len_limit = 2 * (lte_phy_params->N_dft_sz * (lte_phy_params->N_symb_per_subfr - 2) * 4);
	if (subfr_len_limit < RATE * (BLOCK_SIZE + 4))
	{
		for (i = 0; i < TURBO_INT_K_TABLE_SIZE; i++)
		{
			if ((TURBO_INT_K_TABLE[i] + 4) * RATE > subfr_len_limit)
			{
				lte_phy_params->data_len_per_subfr = TURBO_INT_K_TABLE[i - 1];
				break;
			}
		}
	}
	else
	{
		num_blks = subfr_len_limit / (RATE * (BLOCK_SIZE + 4));
		lte_phy_params->data_len_per_subfr = num_blks * BLOCK_SIZE;
	}

	/*
	 * When there is any input length, i.e., determined by transmitted file, its length
	 * should be cut into slices gauged by "lte_phy_params->data_len_per_subfr".
	 */

	lte_phy_iolen(lte_phy_params);
}
