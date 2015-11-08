/*****************************************************************************
 * search.h                                                                  *
 *                         Header file for search.c                          *
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

#ifndef __SEARCH_H
#define __SEARCH_H

typedef struct {
  /* PARAMETERS DEFINING THE SEARCH (THESE GO IN THE SEARCH CONTEXT FILE) */
  unsigned char plaintextVector[256/8];
  unsigned char plaintextXorMask[8];
  unsigned char ciphertext0[8];
  unsigned char ciphertext1[8];
  unsigned char plaintextByteMask;
  unsigned char searchInfo;

  /* PARAMETERS ABOUT THE SEARCH PROCESS */
  long totalUnits;                                     /* total search units */
  long nextUnstartedKeyBlock;                            /* top 24 bits only */
  long totalFinishedKeyBlocks;             /* number of completed key blocks */
  long totalUnstartedKeyBlocks;            /* number of blocks left to start */
  long totalPendingKeyBlocks;                    /* number of blocks running */
} SEARCH_CTX;

#endif

