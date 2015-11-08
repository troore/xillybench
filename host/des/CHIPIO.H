/*****************************************************************************
 * chipio.h                                                                  *
 *                         Header file for chipio.c                          *
 *                                                                           *
 *    Written 1998 by Cryptography Research (http://www.cryptography.com)    *
 *       and Paul Kocher for the Electronic Frontier Foundation (EFF).       *
 *       Placed in the public domain by Cryptography Research and EFF.       *
 *  THIS IS UNSUPPORTED FREE SOFTWARE. USE AND DISTRIBUTE AT YOUR OWN RISK.  *
 *                                                                           *
 *  IMPORTANT: U.S. LAW MAY REGULATE THE USE AND/OR EXPORT OF THIS PROGRAM.  *
 *                                                                           *
 *****************************************************************************
 *                                                                           *
 *   REVISION HISTORY:                                                       *
 *                                                                           *
 *   Version 1.0:  Initial release by Cryptography Research to EFF.          *
 *                                                                           *
 *****************************************************************************/

#ifndef __CHIPIO_H
#define __CHIPIO_H

#define SEARCH_UNITS_PER_CHIP 24

#define REG_PTXT_VECTOR      (0x00)
#define REG_PTXT_XOR_MASK    (0x20)
#define REG_CIPHERTEXT0      (0x28)
#define REG_CIPHERTEXT1      (0x30)
#define REG_PTXT_BYTE_MASK   (0x38)
#define REG_SEARCHINFO       (0x3F)
#define REG_SEARCH_KEY(x)    (0x40 + 8*(x))
#define REG_SEARCH_STATUS(x) (0x47+8*(x))

void SetBaseAddress(int address);
int ResetBoard(int board);
void SetRegister(int board, int chip, int reg, int value);
int GetRegister(int board, int chip, int reg);
int CheckRegister(int board, int chip, int reg, int value);

#endif

