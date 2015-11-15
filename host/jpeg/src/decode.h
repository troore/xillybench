/*
 *  Header file for decoding
 *
 *  @(#) $Id: decode.h,v 1.2 2003/07/18 10:19:21 honda Exp $
 */
#ifndef __DECODE_H_
#define __DECODE_H_

#define NUM_HUFF_TBLS       2
#define NUM_QUANT_TBLS      4
#define DCTSIZE             8
#define DCTSIZE2           64

/*
 * Fix the number of components as 3
 */
#define NUM_COMPONENT       3

#define RGB_NUM  3

/*
 * Define the sample precision as 8
 */
#define IDCT_SHIFT 128
#define IDCT_BOUNT 255
#define MARKER_MARKER 0xff

/* SAMPLING_FACTOR */
#define SF1_1_1  0
#define SF4_1_1  2

void IZigzagMatrix (int *imatrix, int *omatrix);
void IQuantize (int *matrix, unsigned int *qmatrix);
void PostshiftIDctMatrix (int *matrix, int shift);
void BoundIDctMatrix (int *matrix, int Bound);
void WriteBlock (int *store, int *p_out_vpos, int *p_out_hpos,
		unsigned char *p_out_buf);
void Write4Blocks (int *store1, int *store2, int *store3, int *store4,
		int *p_out_vpos, int *p_out_hpos, unsigned char *p_out_buf);
void YuvToRgb (int p, int *y_buf, int *u_buf, int *v_buf);
void decode_block (int comp_no, int *out_buf, int *HuffBuff);
void decode_start (int *out_data_image_width, int *out_data_image_height,
		int *out_data_comp_vpos, int *out_data_comp_hpos);

extern char p_jinfo_data_precision;
extern short p_jinfo_image_height;
extern short p_jinfo_image_width;
extern char p_jinfo_num_components;
extern int p_jinfo_smp_fact;

extern char p_jinfo_comps_info_index[NUM_COMPONENT];
extern char p_jinfo_comps_info_id[NUM_COMPONENT];
extern char p_jinfo_comps_info_h_samp_factor[NUM_COMPONENT];
extern char p_jinfo_comps_info_v_samp_factor[NUM_COMPONENT];
extern char p_jinfo_comps_info_quant_tbl_no[NUM_COMPONENT];
extern char p_jinfo_comps_info_dc_tbl_no[NUM_COMPONENT];
extern char p_jinfo_comps_info_ac_tbl_no[NUM_COMPONENT];

extern unsigned int p_jinfo_quant_tbl_quantval[NUM_QUANT_TBLS][DCTSIZE2];

extern int p_jinfo_dc_xhuff_tbl_bits[NUM_HUFF_TBLS][36];
extern int p_jinfo_dc_xhuff_tbl_huffval[NUM_HUFF_TBLS][257];

extern int p_jinfo_ac_xhuff_tbl_bits[NUM_HUFF_TBLS][36];
extern int p_jinfo_ac_xhuff_tbl_huffval[NUM_HUFF_TBLS][257];

extern int p_jinfo_dc_dhuff_tbl_ml[NUM_HUFF_TBLS];
extern int p_jinfo_dc_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36];
extern int p_jinfo_dc_dhuff_tbl_mincode[NUM_HUFF_TBLS][36];
extern int p_jinfo_dc_dhuff_tbl_valptr[NUM_HUFF_TBLS][36];

extern int p_jinfo_ac_dhuff_tbl_ml[NUM_HUFF_TBLS];
extern int p_jinfo_ac_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36];
extern int p_jinfo_ac_dhuff_tbl_mincode[NUM_HUFF_TBLS][36];
extern int p_jinfo_ac_dhuff_tbl_valptr[NUM_HUFF_TBLS][36];

extern int p_jinfo_MCUWidth;
extern int p_jinfo_MCUHeight;
extern int p_jinfo_NumMCU;

extern unsigned char *p_jinfo_jpeg_data;

#endif /* !__DECODE_H_ */
