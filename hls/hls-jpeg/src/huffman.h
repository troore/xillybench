#ifndef __HUFFMAN_H_
#define __HUFFMAN_H_

int buf_getv (int n);
int huff_make_dhuff_tb (int *p_xhtbl_bits, int p_dhtbl_ml,
		int *p_dhtbl_maxcode, int *p_dhtbl_mincode,
		int *p_dhtbl_valptr);

int buf_getb ();

int DecodeHuffman (int *Xhuff_huffval, int Dhuff_ml,
		int *Dhuff_maxcode, int *Dhuff_mincode, int *Dhuff_valptr);

void DecodeHuffMCU (int out_buf[DCTSIZE2], int num_cmp);

extern unsigned char *CurHuffReadBuf;

#endif
