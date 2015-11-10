
#include "lte_phy.h"
//#include "GeneralFunc.h"
#include "Equalizer.h"
#include "ResMapper.h"
#include "OFDM.h"

void equalizer_chain(LTE_PHY_PARAMS *lte_phy_params)
{
//	SubCarrierMapping(lte_phy_params->resm_in, lte_phy_params->resm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_tx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);
//	ofmodulating(lte_phy_params->resm_out, lte_phy_params->ofmod_out, lte_phy_params->N_tx_ant, lte_phy_params->N_fft_sz, lte_phy_params->N_samps_cp_l_0);
	ofdemodulating(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out, lte_phy_params->N_rx_ant, lte_phy_params->N_fft_sz, lte_phy_params->N_samps_cp_l_0);
	SubCarrierDemapping(lte_phy_params->ofdemod_out, lte_phy_params->resdm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_rx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);
	Equalizing(lte_phy_params->resdm_out, lte_phy_params->eq_out, lte_phy_params->N_dft_sz, lte_phy_params->N_tx_ant, lte_phy_params->N_rx_ant);
}

void equalizer_indep(LTE_PHY_PARAMS *lte_phy_params)
{
	ofdemodulating(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out, lte_phy_params->N_rx_ant, lte_phy_params->N_fft_sz, lte_phy_params->N_samps_cp_l_0);
	SubCarrierDemapping(lte_phy_params->resdm_in, lte_phy_params->resdm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_rx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);
	Equalizing(lte_phy_params->eq_in, lte_phy_params->eq_out, lte_phy_params->N_dft_sz, lte_phy_params->N_tx_ant, lte_phy_params->N_rx_ant);
}

