/*****************************************************************************
 * keyblock.c                                                                *
 *              Key Block & Search Context Management Functions              *
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

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <ctype.h>
#include "search.h"
#include "keyblock.h"

static const char fileHeader[] = "This is a binary file containing the "
        "parameters for a DES search followed by 2^24 bits "
        "indicating which regions of keyspace are left to search.\n\032";

#define CTX_FILE_KEYBLOCKS_OFFSET (sizeof(fileHeader) + 58)
#define MAX_KEY_REGION (1L<<24)          /* 2^56 keys / 2^32 keys per region */
static void EXIT_ERR(char *s) { fprintf(stderr, s); exit(1); }

static void WriteParams(FILE *fp, SEARCH_CTX *ctx);
static void ReadParams(FILE *fp, SEARCH_CTX *ctx);


/*
 *  Create a new search context file from a SEARCH_CTX structure
 */
void WriteSearchContext(FILE *fp, SEARCH_CTX *ctx) {
  unsigned char temp[1024/8];
  long i;

  fwrite(fileHeader, 1, sizeof(fileHeader), fp);
  WriteParams(fp, ctx);
  memset(temp, 255, 1024/8);
  for (i = 0; i < MAX_KEY_REGION/1024; i++)
    fwrite(temp, 1, sizeof(temp), fp);
}


/*
 *  Read search params from a FILE_STRUCTURE and get ready for
 *    calls to ReserveKeyRegion and FinishKeyRegion.
 */
void OpenSearchContext(FILE *fp, SEARCH_CTX *ctx) {
  long blocksLeft, n;
  int i;
  int c;

  rewind(fp);
  for (i = 0; i < sizeof(fileHeader); i++)
    if (fgetc(fp) != fileHeader[i])
      EXIT_ERR("Bad file header in search context file.\n");

  ReadParams(fp, ctx);
  if (ftell(fp) != CTX_FILE_KEYBLOCKS_OFFSET)
    EXIT_ERR("Internal error: File length mismatch.");

  /* INITIALIZE THE SEARCH PROCESS PARAMETERS (except for totalUnits) */
  ctx->nextUnstartedKeyBlock = 0;
  ctx->totalFinishedKeyBlocks = 0;
  ctx->totalUnstartedKeyBlocks = MAX_KEY_REGION;
  ctx->totalPendingKeyBlocks = 0;

  /* FIND OUT HOW MANY KEY BLOCKS ARE LEFT */
  blocksLeft = 0;
  for (n = 0; n < MAX_KEY_REGION/8; n++) {
    c = fgetc(fp);
    if (c < 0 || c > 255)
      EXIT_ERR("Error or premature EOF reading search context file.\n");
    blocksLeft += (c&128)/128 + (c&64)/64 + (c&32)/32 + (c&16)/16 +
           (c&8)/8 + (c&4)/4 + (c&2)/2 + (c&1);
  }
  ctx->totalUnstartedKeyBlocks = blocksLeft;
  ctx->totalFinishedKeyBlocks =  MAX_KEY_REGION - blocksLeft;
}


/*
 *  Reserve a key region to search.  When done searching it, the program
 *  should call FinishKeyRegion.  This function hands out blocks sequentially,
 *  starting with the first unsearched one in the file context file.
 *  If all blocks have been allocated and no free ones are left, the
 *  function returns (-1).
 */
long ReserveKeyRegion(FILE *fp, SEARCH_CTX *ctx) {
  int c,b;

  if (ctx->nextUnstartedKeyBlock >= MAX_KEY_REGION)
    return(-1);
  if (fseek(fp, CTX_FILE_KEYBLOCKS_OFFSET + ctx->nextUnstartedKeyBlock/8,
          SEEK_SET))
    EXIT_ERR("Error seeking search context file.\n");
  if ((ctx->nextUnstartedKeyBlock & 7) != 0)
    c = fgetc(fp);
  while (ctx->nextUnstartedKeyBlock < MAX_KEY_REGION) {
    b = (int)(ctx->nextUnstartedKeyBlock & 7);
    if (b == 0)
      c = fgetc(fp);
    if (c < 0 || c > 255)
      EXIT_ERR("Error reading from search context file.\n");
    if (b == 0 && c == 0) {
      ctx->nextUnstartedKeyBlock += 8;
      continue;
    }
    if ((c << b) & 128)
      break;
    ctx->nextUnstartedKeyBlock++;
  }
  if (ctx->nextUnstartedKeyBlock >= MAX_KEY_REGION)
    return (-1);
  ctx->totalUnstartedKeyBlocks--;
  ctx->totalPendingKeyBlocks++;
  return (ctx->nextUnstartedKeyBlock++);
}


/*
 *  Finish searching a key region by marking it as completed in the contetx
 *  file.
 */
void FinishKeyRegion(FILE *fp, SEARCH_CTX *ctx, long keyRegion) {
  int c,b;

  if (keyRegion < 0 || keyRegion > MAX_KEY_REGION)
    EXIT_ERR("Bad key region\n");
  if (fseek(fp, CTX_FILE_KEYBLOCKS_OFFSET + keyRegion/8, SEEK_SET))
    EXIT_ERR("Error seeking in search context file.\n");
  b = (int)(keyRegion & 7);                               /* b = bit in byte */
  c = getc(fp);
  if (((c << b) & 128) == 0)
    printf("WARNING: FinishKeyRegion called, but region already searched!\n");
  else {
    if (fseek(fp, CTX_FILE_KEYBLOCKS_OFFSET + keyRegion/8, SEEK_SET))
      EXIT_ERR("Error seeking in search context file.\n");
    fputc(c & (255 ^ (128>>b)), fp);
    fflush(fp);
    ctx->totalFinishedKeyBlocks++;
    ctx->totalPendingKeyBlocks--;
  }
}


/*
 *  Write a SEARCH_CTX structure to a FILE*
 */
static void WriteParams(FILE *fp, SEARCH_CTX *ctx) {
  fwrite(ctx->plaintextVector, 1, 32, fp);                       /* 32 bytes */
  fwrite(ctx->plaintextXorMask, 1, 8, fp);                       /*  8 bytes */
  fwrite(ctx->ciphertext0, 1, 8, fp);                            /*  8 bytes */
  fwrite(ctx->ciphertext1, 1, 8, fp);                            /*  8 bytes */
  fwrite(&(ctx->plaintextByteMask), 1, 1, fp);                   /*  1 byte  */
  fwrite(&(ctx->searchInfo), 1, 1, fp);                          /*  1 byte  */
}

/*
 *  Read a SEARCH_CTX structure from a FILE*
 */
static void ReadParams(FILE *fp, SEARCH_CTX *ctx) {
  fread(ctx->plaintextVector, 1, 32, fp);                        /* 32 bytes */
  fread(ctx->plaintextXorMask, 1, 8, fp);                        /*  8 bytes */
  fread(ctx->ciphertext0, 1, 8, fp);                             /*  8 bytes */
  fread(ctx->ciphertext1, 1, 8, fp);                             /*  8 bytes */
  fread(&(ctx->plaintextByteMask), 1, 1, fp);                    /*  1 byte  */
  fread(&(ctx->searchInfo), 1, 1, fp);                           /*  1 byte  */
}

