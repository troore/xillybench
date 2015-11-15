/*
 * Read the header information from buffer in JFIF format and begin decoding
 *
 *  @(#) $Id: jfif_read.c,v 1.2 2003/07/18 10:19:21 honda Exp $ 
 */

#include "decode.h"
#include "jpeg2bmp.h"
#include "huffman.h"
#include "marker.h"

/*
 * Initialize after reading markers
 */
	void
jpeg_init_decompress ()
{
	int tmp;
	/*
	 * Get MCU number
	 */
	p_jinfo_MCUHeight = (p_jinfo_image_height - 1) / 8 + 1;
	p_jinfo_MCUWidth = (p_jinfo_image_width - 1) / 8 + 1;
	p_jinfo_NumMCU = p_jinfo_MCUHeight * p_jinfo_MCUWidth;

	/*
	 * Create Huffman Table for decoding
	 */
	tmp = huff_make_dhuff_tb (&p_jinfo_dc_xhuff_tbl_bits[0][0],
			p_jinfo_dc_dhuff_tbl_ml[0],
			&p_jinfo_dc_dhuff_tbl_maxcode[0][0],
			&p_jinfo_dc_dhuff_tbl_mincode[0][0],
			&p_jinfo_dc_dhuff_tbl_valptr[0][0]);
	p_jinfo_dc_dhuff_tbl_ml[0] = tmp;
	tmp = huff_make_dhuff_tb (&p_jinfo_dc_xhuff_tbl_bits[1][0],
			p_jinfo_dc_dhuff_tbl_ml[1],
			&p_jinfo_dc_dhuff_tbl_maxcode[1][0],
			&p_jinfo_dc_dhuff_tbl_mincode[1][0],
			&p_jinfo_dc_dhuff_tbl_valptr[1][0]);
	p_jinfo_dc_dhuff_tbl_ml[1] = tmp;
	tmp = huff_make_dhuff_tb (&p_jinfo_ac_xhuff_tbl_bits[0][0],
			p_jinfo_ac_dhuff_tbl_ml[0],
			&p_jinfo_ac_dhuff_tbl_maxcode[0][0],
			&p_jinfo_ac_dhuff_tbl_mincode[0][0],
			&p_jinfo_ac_dhuff_tbl_valptr[0][0]);
	p_jinfo_ac_dhuff_tbl_ml[0] = tmp;
	tmp = huff_make_dhuff_tb (&p_jinfo_ac_xhuff_tbl_bits[1][0],
			p_jinfo_ac_dhuff_tbl_ml[1],
			&p_jinfo_ac_dhuff_tbl_maxcode[1][0],
			&p_jinfo_ac_dhuff_tbl_mincode[1][0],
			&p_jinfo_ac_dhuff_tbl_valptr[1][0]);
	p_jinfo_ac_dhuff_tbl_ml[1] = tmp;
}

	void
jpeg_read (unsigned char *read_buf)
{

	/*
	 * Read markers
	 */
	read_markers (read_buf);


	/*
	 * Initialize the information used for decoding
	 */
	jpeg_init_decompress ();

	/*
	 * Start decoding
	 */
	decode_start (&OutData_image_width, &OutData_image_height,
			&OutData_comp_vpos[0], &OutData_comp_hpos[0]);

}
