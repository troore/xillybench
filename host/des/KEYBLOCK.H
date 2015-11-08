/*****************************************************************************
 * keyblock.h                                                                *
 *                        Header file for keyblock.c                         *
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

#ifndef __KEYBLOCK_H
#define __KEYBLOCK_H

void WriteSearchContext(FILE *fp, SEARCH_CTX *sp);
void OpenSearchContext(FILE *fp, SEARCH_CTX *ctx);
long ReserveKeyRegion(FILE *fp, SEARCH_CTX *ctx);
void FinishKeyRegion(FILE *fp, SEARCH_CTX *ctx, long keyRegion);

#endif

