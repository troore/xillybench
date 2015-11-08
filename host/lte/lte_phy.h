#ifndef __LTE_PHY_H_
#define __LTE_PHY_H_

//#include <iostream>
#include <complex>

/******************************************
DEFINES
*******************************************/

// FFT sizes
#define LTE_PHY_FFT_SIZE_1_92MHZ  128
#define LTE_PHY_FFT_SIZE_3_84MHZ  256
#define LTE_PHY_FFT_SIZE_7_68MHZ  512
#define LTE_PHY_FFT_SIZE_15_36MHZ 1024
#define LTE_PHY_FFT_SIZE_23_04MHZ 1536
#define LTE_PHY_FFT_SIZE_30_72MHZ 2048
#define LTE_PHY_FFT_SIZE_MAX (LTE_PHY_FFT_SIZE_30_72MHZ)

// DFT sizes
#define LTE_PHY_DFT_SIZE_1_92MHZ  75
#define LTE_PHY_DFT_SIZE_3_84MHZ  150
#define LTE_PHY_DFT_SIZE_7_68MHZ  300
#define LTE_PHY_DFT_SIZE_15_36MHZ 600
#define LTE_PHY_DFT_SIZE_23_04MHZ 900
#define LTE_PHY_DFT_SIZE_30_72MHZ 1200
#define LTE_PHY_DFT_SIZE_MAX (LTE_PHY_DFT_SIZE_30_72MHZ)

// N_rb_ul
#define LTE_PHY_N_RB_UL_1_4MHZ 6
#define LTE_PHY_N_RB_UL_3MHZ   15
#define LTE_PHY_N_RB_UL_5MHZ   25
#define LTE_PHY_N_RB_UL_10MHZ  50
#define LTE_PHY_N_RB_UL_15MHZ  75
#define LTE_PHY_N_RB_UL_20MHZ  100
#define LTE_PHY_N_RB_UL_MAX    110

// N_sc_rb
#define LTE_PHY_N_SC_RB_UL 12

// Maximum number of antennas
#define LTE_PHY_N_ANT_MAX 4

// Symbol, CP, Slot, Subframe, and Frame timing
// Generic
#define LTE_PHY_SFN_MAX           1023
#define LTE_PHY_N_SLOTS_PER_SUBFR 2
#define LTE_PHY_N_SUBFR_PER_FRAME 10
#define LTE_PHY_N_SYMB_PER_SLOT 7
#define LTE_PHY_N_SYMB_PER_SUBFR (LTE_PHY_N_SLOTS_PER_SUBFR * LTE_PHY_N_SYMB_PER_SLOT)

// 1.25MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_1_92MHZ  128
#define LTE_PHY_N_SAMPS_CP_L_0_1_92MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_1_92MHZ 9

// 2.5MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_3_84MHZ  256
#define LTE_PHY_N_SAMPS_CP_L_0_3_84MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_3_84MHZ 9

// 5MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_7_68MHZ  512
#define LTE_PHY_N_SAMPS_CP_L_0_7_68MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_7_68MHZ 9

// 10MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_15_36MHZ  1024
#define LTE_PHY_N_SAMPS_CP_L_0_15_36MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_15_36MHZ 9

// 15MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_23_04MHZ  1536
#define LTE_PHY_N_SAMPS_CP_L_0_23_04MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_23_04MHZ 9

// 20MHz bandwidths
#define LTE_PHY_N_SAMPS_PER_SYMB_30_72MHZ  2048
#define LTE_PHY_N_SAMPS_CP_L_0_30_72MHZ    9
#define LTE_PHY_N_SAMPS_CP_L_ELSE_30_72MHZ 9
//#define LTE_PHY_N_SAMPS_PER_SLOT_30_72MHZ  15360
//#define LTE_PHY_N_SAMPS_PER_SUBFR_30_72MHZ (LTE_PHY_N_SAMPS_PER_SLOT_30_72MHZ * LTE_PHY_N_SLOTS_PER_SUBFR)
//#define LTE_PHY_N_SAMPS_PER_FRAME_30_72MHZ (LTE_PHY_N_SAMPS_PER_SUBFR_30_72MHZ * LTE_PHY_N_SUBFR_PER_FRAME)


typedef enum{
    LTE_PHY_FS_1_92MHZ = 0, // 1.25MHz bandwidth
    LTE_PHY_FS_3_84MHZ,     // 2.5MHz bandwidth
    LTE_PHY_FS_7_68MHZ,     // 5MHz bandwidth
    LTE_PHY_FS_15_36MHZ,    // 10MHz bandwidth
//	LTE_PHY_FS_23_04MHZ,		// 15MHz bandwidth
    LTE_PHY_FS_30_72MHZ = 4,    // 20MHz bandwidths
	LTE_PHY_FS_TEST = 5,
    LTE_PHY_FS_N_ITEMS,
}LTE_PHY_FS_ENUM;

/*
 * Modulation parameters
 */
enum {
	LTE_BPSK = 0,
	LTE_QPSK,
	LTE_QAM16,
	LTE_QAM64
};

// Table length
#define BPSK_TABLE_LEN 2
#define QPSK_TABLE_LEN 4
#define	QAM16_TABLE_LEN 16
#define QAM64_TABLE_LEN 64
#define MAX_MOD_TABLE_LEN (QAM64_TABLE_LEN)

// Modulation type and number of bits mapping
#define BPSK_BITS_PER_SAMP 1
#define QPSK_BITS_PER_SAMP 2
#define QAM16_BITS_PER_SAMP 4
#define QAM64_BITS_PER_SAMP 6
#define MAX_MOD_BITS_PER_SAMP (QAM64_BITS_PER_SAMP)

// Turbo
#define TURBO_INT_K_TABLE_SIZE 188
#define BLOCK_SIZE	6144
#define RATE 3
#define N_GENS 2 // number of generators
#define CST_LEN 4 // constraint length
#define N_TAIL (CST_LEN - 1)
#define N_STATES (1 << N_TAIL)
#define N_UNCODED (BLOCK_SIZE)
#define N_CODED (1 + (N_GENS - 1) * 2) * N_UNCODED + 2 * N_GENS * N_TAIL
#define MAX_ITERATIONS 3 // turbo decoder maximum interation times


typedef struct
{
	// General parameters
	int fs; /* sampling rate */
	int N_samps_per_symb; /* FFT size */
	int N_dft_sz; /* DFT size for uplink SC-FDMA */
	int N_fft_sz; /* DFT size for uplink SC-FDMA */
	int N_samps_cp_l_0; /* cyclic prefix length for the 1st OFDM symbol per slot */
 	int N_samps_cp_l_else; /* normal cyclic prefix length */
	int N_samps_per_slot;
	int N_samps_per_subfr;
	int N_samps_per_frame;
	int N_bits_per_samp;

	int N_symb_per_subfr;

	int N_tx_ant;
	int N_rx_ant;

	int mod_type;

	int sc_loc;
	int dmrs_symb_pos[2];

	// input/output length
	int data_len_per_subfr; // should be determined by the capacity of a single sub-frame
	// Turbo
#define N_TURBO_OUT_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_30_72MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * QAM64_BITS_PER_SAMP)
#define N_TURBO_IN_MAX (N_TURBO_OUT_MAX / RATE)
	int te_in[N_TURBO_IN_MAX];
	int te_out[N_TURBO_OUT_MAX];
	float td_in[N_TURBO_OUT_MAX];
	int td_out[N_TURBO_IN_MAX];
	int te_in_buf_sz;
	int te_out_buf_sz;
	int td_in_buf_sz;
	int td_out_buf_sz;

	// Rate matching
#define N_RM_OUT_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_30_72MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * QAM64_BITS_PER_SAMP)
#define N_RM_IN_MAX (N_RM_OUT_MAX)
	int rm_in[N_RM_IN_MAX];
	int rm_out[N_RM_OUT_MAX];
	float rdm_in[N_RM_OUT_MAX];
	float rdm_out[N_RM_IN_MAX];
	int rdm_hard[N_RM_IN_MAX];
	int rm_in_buf_sz;
	int rm_out_buf_sz;
	int rdm_in_buf_sz;
	int rdm_out_buf_sz;

	// Scrambling
#define N_SCRAMB_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_30_72MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * QAM64_BITS_PER_SAMP)
#define N_SCRAMB_OUT_MAX (N_SCRAMB_IN_MAX)
	int scramb_in[N_SCRAMB_IN_MAX];
	int scramb_out[N_SCRAMB_OUT_MAX];
	float descramb_in[N_SCRAMB_IN_MAX];
	float descramb_out[N_SCRAMB_OUT_MAX];
	int scramb_seq_int[N_SCRAMB_IN_MAX];
	int scramb_in_buf_sz;
	int scramb_out_buf_sz;
	int descramb_in_buf_sz;
	int descramb_out_buf_sz;

	// Modulation
#define N_MOD_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * MAX_MOD_BITS_PER_SAMP)
#define N_MOD_OUT_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * (LTE_PHY_N_SYMB_PER_SUBFR - 2))
	int mod_in[N_MOD_IN_MAX];
//	std::complex<float> mod_out[N_MOD_OUT_MAX];
	float mod_out[N_MOD_OUT_MAX * 2];
	float mod_out_real[N_MOD_OUT_MAX];
	float mod_out_imag[N_MOD_OUT_MAX];
//	std::complex<float> demod_in[N_MOD_OUT_MAX];
	float demod_in[N_MOD_OUT_MAX * 2];
	float demod_in_real[N_MOD_IN_MAX];
	float demod_in_imag[N_MOD_IN_MAX];
	float demod_LLR[N_MOD_IN_MAX];
	int demod_HD[N_MOD_IN_MAX];
	int mod_in_buf_sz;
	int mod_out_buf_sz;
	int demod_in_buf_sz;
	int demod_out_buf_sz;

	
	// Transform precoding
#define N_TRANS_ENCODER_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * (LTE_PHY_N_SYMB_PER_SUBFR - 2))
#define N_TRANS_ENCODER_OUT_MAX (N_TRANS_ENCODER_IN_MAX)
#define N_TRANS_DECODER_IN_MAX (N_TRANS_ENCODER_IN_MAX)
#define N_TRANS_DECODER_OUT_MAX (N_TRANS_ENCODER_IN_MAX)

	/*
	  std::complex<float> trans_encoder_in_cplx[N_TRANS_ENCODER_IN_MAX];
	  std::complex<float> trans_encoder_out_cplx[N_TRANS_ENCODER_OUT_MAX];
	  std::complex<float> trans_decoder_in_cplx[N_TRANS_DECODER_IN_MAX];
	  std::complex<float> trans_decoder_out_cplx[N_TRANS_DECODER_OUT_MAX];
	*/
	
	float trans_encoder_in[N_TRANS_ENCODER_IN_MAX * 2];
	float trans_encoder_out[N_TRANS_ENCODER_OUT_MAX * 2];
	float trans_decoder_in[N_TRANS_DECODER_IN_MAX * 2];
	float trans_decoder_out[N_TRANS_DECODER_OUT_MAX * 2];
 
	float trans_encoder_in_real[N_TRANS_ENCODER_IN_MAX];
	float trans_encoder_in_imag[N_TRANS_ENCODER_IN_MAX];
	float trans_encoder_out_real[N_TRANS_ENCODER_OUT_MAX];
	float trans_encoder_out_imag[N_TRANS_ENCODER_OUT_MAX];
	float trans_decoder_in_real[N_TRANS_DECODER_IN_MAX];
	float trans_decoder_in_imag[N_TRANS_DECODER_IN_MAX];
	float trans_decoder_out_real[N_TRANS_DECODER_OUT_MAX];
	float trans_decoder_out_imag[N_TRANS_DECODER_OUT_MAX];

	int trans_encoder_in_buf_sz;
	int trans_encoder_out_buf_sz;
	int trans_decoder_in_buf_sz;
	int trans_decoder_out_buf_sz;

	// Resource mapping
#define N_RESMAPPER_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_30_72MHZ * (LTE_PHY_N_SYMB_PER_SUBFR - 2))
#define N_RESMAPPER_OUT_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_FFT_SIZE_30_72MHZ * LTE_PHY_N_SYMB_PER_SUBFR)
#define N_RESDEMAPPER_IN_MAX (N_RESMAPPER_OUT_MAX)
#define N_RESDEMAPPER_OUT_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_30_72MHZ * LTE_PHY_N_SYMB_PER_SUBFR)
	/*
	  std::complex<float> resm_in[N_RESMAPPER_IN_MAX];
	  std::complex<float> resm_out[N_RESMAPPER_OUT_MAX];
	  std::complex<float> resdm_in[N_RESDEMAPPER_IN_MAX];
	  std::complex<float> resdm_out[N_RESDEMAPPER_OUT_MAX];
	*/
	
	float resm_in[2 * N_RESMAPPER_IN_MAX];
	float resm_out[2 * N_RESMAPPER_OUT_MAX];
	float resdm_in[2 * N_RESDEMAPPER_IN_MAX];
	float resdm_out[2 * N_RESDEMAPPER_OUT_MAX];
	float resm_in_real[N_RESMAPPER_IN_MAX];
	float resm_in_imag[N_RESMAPPER_IN_MAX];
	float resm_out_real[N_RESMAPPER_OUT_MAX];
	float resm_out_imag[N_RESMAPPER_OUT_MAX];
	float resdm_in_real[N_RESDEMAPPER_IN_MAX];
	float resdm_in_imag[N_RESDEMAPPER_IN_MAX];
	float resdm_out_real[N_RESDEMAPPER_OUT_MAX];
	float resdm_out_imag[N_RESDEMAPPER_OUT_MAX];
	int resm_in_buf_sz;
	int resm_out_buf_sz;
	int resdm_in_buf_sz;
	int resdm_out_buf_sz;

	// Equalizing
#define N_EQ_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_N_SYMB_PER_SUBFR * LTE_PHY_DFT_SIZE_MAX)
#define N_EQ_OUT_MAX (LTE_PHY_N_ANT_MAX * (LTE_PHY_N_SYMB_PER_SUBFR - 2) * LTE_PHY_DFT_SIZE_MAX)
//	std::complex<float> eq_in[N_EQ_IN_MAX];
//	std::complex<float> eq_out[N_EQ_OUT_MAX];
	float eq_in[N_EQ_IN_MAX * 2];
	float eq_out[N_EQ_OUT_MAX * 2];
	float eq_in_real[N_EQ_IN_MAX];
	float eq_in_imag[N_EQ_IN_MAX];
	float eq_out_real[N_EQ_OUT_MAX];
	float eq_out_imag[N_EQ_OUT_MAX];
	float DMRS[2 * LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2];
	float DMRSReal[LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2];
	float DMRSImag[LTE_PHY_N_ANT_MAX * LTE_PHY_DFT_SIZE_MAX * 2];
	int eq_in_buf_sz;
	int eq_out_buf_sz;

	// OFDM
#define N_OFMOD_IN_MAX (LTE_PHY_N_ANT_MAX * LTE_PHY_FFT_SIZE_MAX * LTE_PHY_N_SYMB_PER_SUBFR)
#define N_OFMOD_OUT_MAX (LTE_PHY_N_ANT_MAX * (LTE_PHY_FFT_SIZE_MAX + LTE_PHY_N_SAMPS_CP_L_0_30_72MHZ) * LTE_PHY_N_SYMB_PER_SUBFR)
#define N_OFDEMOD_IN_MAX (N_OFMOD_OUT_MAX)
#define N_OFDEMOD_OUT_MAX (N_OFMOD_IN_MAX)
	/*
	  std::complex<float> ofmod_in[N_OFMOD_IN_MAX];
	  std::complex<float> ofmod_out[N_OFMOD_OUT_MAX];
	  std::complex<float> ofdemod_in[N_OFDEMOD_IN_MAX];
	  std::complex<float> ofdemod_out[N_OFDEMOD_OUT_MAX];
	*/
	float ofmod_in[N_OFMOD_IN_MAX * 2];
	float ofmod_out[N_OFMOD_OUT_MAX * 2];
	float ofdemod_in[N_OFDEMOD_IN_MAX * 2];
	float ofdemod_out[N_OFDEMOD_OUT_MAX * 2];
	float ofmod_in_real[N_OFMOD_IN_MAX];
	float ofmod_in_imag[N_OFMOD_IN_MAX];
	float ofmod_out_real[N_OFMOD_OUT_MAX];
	float ofmod_out_imag[N_OFMOD_OUT_MAX];
	float ofdemod_in_real[N_OFDEMOD_IN_MAX];
	float ofdemod_in_imag[N_OFDEMOD_IN_MAX];
	float ofdemod_out_real[N_OFDEMOD_OUT_MAX];
	float ofdemod_out_imag[N_OFDEMOD_OUT_MAX];
	int ofmod_in_buf_sz;
	int ofmod_out_buf_sz;
	int ofdemod_in_buf_sz;
	int ofdemod_out_buf_sz;

} LTE_PHY_PARAMS;

/*
typedef struct {
	LTE_PHY_PARAMS *lte_phy_params;
	float *pInpData;
	float *pOutData;
} KERNEL_THREAD_PARAMS;
*/

extern int TURBO_INT_K_TABLE[TURBO_INT_K_TABLE_SIZE];
extern int TURBO_INT_F1_TABLE[TURBO_INT_K_TABLE_SIZE];
extern int TURBO_INT_F2_TABLE[TURBO_INT_K_TABLE_SIZE];

// Function declarations
void lte_phy_init(LTE_PHY_PARAMS *lte_phy_params, int fs_id, int mod_type, int n_tx_ant, int n_rx_ant);

//void lte_phy_init(LTE_PHY_PARAMS *lte_phy_params, int fs_id);

void lte_phy_usage_info(char *exec);

void lte_phy_iolen(LTE_PHY_PARAMS *lte_phy_params);

#endif
