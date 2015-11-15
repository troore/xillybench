
/*
 *  Decoder
 *
 *  @(#) $Id: decode.c,v 1.2 2003/07/18 10:19:21 honda Exp $
 */
#include <stdio.h>

#include "decode.h"
#include "huffman.h"
#include "jpeg2bmp.h"
#include "chenidct.h"

char p_jinfo_data_precision;
short p_jinfo_image_height;
short p_jinfo_image_width;
char p_jinfo_num_components;
int p_jinfo_smp_fact;

char p_jinfo_comps_info_index[NUM_COMPONENT];
char p_jinfo_comps_info_id[NUM_COMPONENT];
char p_jinfo_comps_info_h_samp_factor[NUM_COMPONENT];
char p_jinfo_comps_info_v_samp_factor[NUM_COMPONENT];
char p_jinfo_comps_info_quant_tbl_no[NUM_COMPONENT];
char p_jinfo_comps_info_dc_tbl_no[NUM_COMPONENT];
char p_jinfo_comps_info_ac_tbl_no[NUM_COMPONENT];

unsigned int p_jinfo_quant_tbl_quantval[NUM_QUANT_TBLS][DCTSIZE2];

int p_jinfo_dc_xhuff_tbl_bits[NUM_HUFF_TBLS][36];
int p_jinfo_dc_xhuff_tbl_huffval[NUM_HUFF_TBLS][257];

int p_jinfo_ac_xhuff_tbl_bits[NUM_HUFF_TBLS][36];
int p_jinfo_ac_xhuff_tbl_huffval[NUM_HUFF_TBLS][257];

int p_jinfo_dc_dhuff_tbl_ml[NUM_HUFF_TBLS];
int p_jinfo_dc_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36];
int p_jinfo_dc_dhuff_tbl_mincode[NUM_HUFF_TBLS][36];
int p_jinfo_dc_dhuff_tbl_valptr[NUM_HUFF_TBLS][36];

int p_jinfo_ac_dhuff_tbl_ml[NUM_HUFF_TBLS];
int p_jinfo_ac_dhuff_tbl_maxcode[NUM_HUFF_TBLS][36];
int p_jinfo_ac_dhuff_tbl_mincode[NUM_HUFF_TBLS][36];
int p_jinfo_ac_dhuff_tbl_valptr[NUM_HUFF_TBLS][36];

int p_jinfo_MCUWidth;
int p_jinfo_MCUHeight;
int p_jinfo_NumMCU;

unsigned char *p_jinfo_jpeg_data;

int rgb_buf[4][RGB_NUM][DCTSIZE2];

/* Is zig-zag map for matrix -> scan array */
const int zigzag_index[64] = {
	0, 1, 5, 6, 14, 15, 27, 28,
	2, 4, 7, 13, 16, 26, 29, 42,
	3, 8, 12, 17, 25, 30, 41, 43,
	9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63
};

/*
 * IZigzagMatrix() performs an inverse zig-zag translation on the
 * input imatrix and places the output in omatrix.
 */
	void
IZigzagMatrix (int imatrix[DCTSIZE2], int omatrix[DCTSIZE2])
{
	int i;
	
	int h;

	for (h = 0; h < 1; h++) {
	for (i = 0; i < DCTSIZE2; i++)
	{
#pragma HLS PIPELINE

	//	*(omatrix++) = imatrix[zigzag_index[i]];
		omatrix[i] = imatrix[zigzag_index[i]];
	}
	}
}


/*
 * IQuantize() takes an input matrix and does an inverse quantization
 * and puts the output int qmatrix.
 */
	void
IQuantize (int matrix[DCTSIZE2], unsigned int qmatrix[DCTSIZE2])
{
	int *mptr;

	for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
	{
#pragma HLS PIPELINE

		*mptr = *mptr * (*qmatrix);
		qmatrix++;
	}

	/*
	int i;
	int matrix_buf[DCTSIZE2];

	int h;

	for (h = 0; h < 1; h++) {
	for (i = 0; i < DCTSIZE2; i++) {
		matrix_buf[i] = matrix[i] * qmatrix[i];
	}
	}
	for (i = 0; i < DCTSIZE2; i++) {
		matrix[i] = matrix_buf[i];
	}
	*/
}


/*
 * PostshiftIDctMatrix() adds 128 (2048) to all 64 elements of an 8x8 matrix.
 * This results in strictly positive values for all pixel coefficients.
 */
	void
PostshiftIDctMatrix (int matrix[DCTSIZE2], int shift)
{
	int *mptr;
	for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
	{
#pragma HLS PIPELINE

		*mptr += shift;
	}

	/*
	int i;
	int matrix_buf[DCTSIZE2];

	int h;

	for (h = 0; h < 100000; h++) {
	for (i = 0; i < DCTSIZE2; i++) {
		matrix_buf[i] = matrix[i] + shift;
	}
	}

	for (i = 0; i < DCTSIZE2; i++) {
		matrix[i] = matrix_buf[i];
	}
	*/
}


/*
 * BoundIDctMatrix bounds the inverse dct matrix so that no pixel has a
 * value greater than 255 (4095) or less than 0.
 */
	void
BoundIDctMatrix (int matrix[DCTSIZE2], int Bound)
{
	int *mptr;

	for (mptr = matrix; mptr < matrix + DCTSIZE2; mptr++)
	{
#pragma HLS PIPELINE

		if (*mptr < 0)
		{
			*mptr = 0;
		}
		else if (*mptr > Bound)
		{
			*mptr = Bound;
		}
	}

	/*
	int i;
	int matrix_buf[DCTSIZE2];

	int h;

	for (h = 0; h < 1; h++) {
	for (i = 0; i < DCTSIZE2; i++) {
		if (matrix[i] < 0)
			matrix_buf[i] = 0;
		else if (matrix[i] > Bound)
			matrix_buf[i] = Bound;
	}
	}
	for (i = 0; i < DCTSIZE2; i++) {
		matrix[i] = matrix_buf[i];
	}
	*/
}


	void
WriteOneBlock (int *store, unsigned char *out_buf, int width, int height,
		int voffs, int hoffs)
{
	int i, e;

	/* Find vertical buffer offs. */
	for (i = voffs; i < voffs + DCTSIZE; i++)
	{
		if (i < height)
		{
			int diff;
			diff = width * i;
			for (e = hoffs; e < hoffs + DCTSIZE; e++)
			{
				if (e < width)
				{
					out_buf[diff + e] = (unsigned char) (*(store++));
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
}

/*
 * WriteBlock() writes an array of data in the integer array pointed to
 * by store out to the driver specified by the IOB.  The integer array is
 * stored in row-major form, that is, the first row of (8) elements, the
 * second row of (8) elements....
 * ONLY for MCU 1:1:1
 */
	void
WriteBlock (int *store, int *p_out_vpos, int *p_out_hpos,
		unsigned char *p_out_buf)
{
	int voffs, hoffs;

	/*
	 * Get vertical offsets
	 */
	voffs = *p_out_vpos * DCTSIZE;
	hoffs = *p_out_hpos * DCTSIZE;

	/*
	 * Write block
	 */
	WriteOneBlock (store,
			p_out_buf,
			p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

	/*
	 *  Add positions
	 */
	(*p_out_hpos)++;
	(*p_out_vpos)++;

	if (*p_out_hpos < p_jinfo_MCUWidth)
	{
		(*p_out_vpos)--;
	}
	else
	{
		*p_out_hpos = 0;		/* If at end of image (width) */
	}
}

/*
 *  4:1:1
 */
	void
Write4Blocks (int *store1, int *store2, int *store3, int *store4,
		int *p_out_vpos, int *p_out_hpos, unsigned char *p_out_buf)
{
	int voffs, hoffs;

	/*
	 * OX
	 * XX
	 */
	voffs = *p_out_vpos * DCTSIZE;
	hoffs = *p_out_hpos * DCTSIZE;
	WriteOneBlock (store1, p_out_buf,
			p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

	/*
	 * XO
	 * XX
	 */
	hoffs += DCTSIZE;
	WriteOneBlock (store2, p_out_buf,
			p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);

	/*
	 * XX
	 * OX
	 */
	voffs += DCTSIZE;
	hoffs -= DCTSIZE;
	WriteOneBlock (store3, p_out_buf,
			p_jinfo_image_width, p_jinfo_image_height, voffs, hoffs);


	/*
	 * XX
	 * XO
	 */
	hoffs += DCTSIZE;
	WriteOneBlock (store4,
			p_out_buf, p_jinfo_image_width, p_jinfo_image_height,
			voffs, hoffs);

	/*
	 * Add positions
	 */
	*p_out_hpos = *p_out_hpos + 2;
	*p_out_vpos = *p_out_vpos + 2;


	if (*p_out_hpos < p_jinfo_MCUWidth)
	{
		*p_out_vpos = *p_out_vpos - 2;
	}
	else
	{
		*p_out_hpos = 0;		/* If at end of image (width) */
	}
}


/*
 * Transform from Yuv into RGB
 */
	void
YuvToRgb (int p, int *y_buf, int *u_buf, int *v_buf)
{
	int r, g, b;
	int y, u, v;
	int i;

	for (i = 0; i < DCTSIZE2; i++)
	{
		y = y_buf[i];
		u = u_buf[i] - 128;
		v = v_buf[i] - 128;

		r = (y * 256 + v * 359 + 128) >> 8;
		g = (y * 256 - u * 88 - v * 182 + 128) >> 8;
		b = (y * 256 + u * 454 + 128) >> 8;

		if (r < 0)
			r = 0;
		else if (r > 255)
			r = 255;

		if (g < 0)
			g = 0;
		else if (g > 255)
			g = 255;

		if (b < 0)
			b = 0;
		else if (b > 255)
			b = 255;

		rgb_buf[p][0][i] = r;
		rgb_buf[p][1][i] = g;
		rgb_buf[p][2][i] = b;

	}
}


/*
 * Decode one block
 */
	void
decode_block (int comp_no, int *out_buf, int *HuffBuff)
{
	double start, finish, elapsed_time;

	int QuantBuff[DCTSIZE2];
	unsigned int *p_quant_tbl;

	p_quant_tbl =
		&p_jinfo_quant_tbl_quantval[(int)p_jinfo_comps_info_quant_tbl_no[comp_no]][DCTSIZE2];

	DecodeHuffMCU (HuffBuff, comp_no);

	IZigzagMatrix (HuffBuff, QuantBuff);

	IQuantize (QuantBuff, p_quant_tbl);

	ChenIDct (QuantBuff, out_buf);

	PostshiftIDctMatrix (out_buf, IDCT_SHIFT);

	BoundIDctMatrix (out_buf, IDCT_BOUND);
}


	void
decode_start (int *out_data_image_width, int *out_data_image_height,
		int *out_data_comp_vpos, int *out_data_comp_hpos)
{
	int i;
//	int CurrentMCU = 0;
	int HuffBuff[DCTSIZE2];
#define FF 1
	int IDCTBuff[FF][DCTSIZE2];


	/* Read buffer */
	CurHuffReadBuf = p_jinfo_jpeg_data;

	/*
	 * Initial value of DC element is 0
	 */
	HuffBuff[0] = 0;

//	printf ("Decode 4:1:1 NumMCU = %d\n", p_jinfo_NumMCU);
	/*
	 * 4_1_1
	 */
	for (i = 0; i < FF; i++)
	{
		decode_block (0, IDCTBuff[i], HuffBuff);
	}

}
