/*****************************************************************************
 * initsrch.c                                                                *
 *                DES Search Engine Search Definition Program                *
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
 *   IMPLEMENTATION NOTES:                                                   *
 *                                                                           *
 *   This program is used to define searches that will be run on the DES     *
 *   search array.  The program creates a search context file containing     *
 *   the ciphertexts, search parameters, and a list of the key regions       *
 *   to search.  (A key region is the top 24 bits of a key.)                 *
 *                                                                           *
 *****************************************************************************
 *                                                                           *
 *   REVISION HISTORY:                                                       *
 *                                                                           *
 *   Version 1.0:  Initial release by Cryptography Research to EFF.          *
 *                                                                           *
 *****************************************************************************/

#define SOFTWARE_VERSION "1.0"
#define SOFTWARE_DATE    "04-21-1998"


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <ctype.h>
#include "search.h"
#include "keyblock.h"

#define EXIT_ERR(s) { fprintf(stderr, s); exit(1); }
static void dumpBin(char *intro, unsigned char *data, int len);
static int unhex(unsigned char *data, char *hex, int byteCount);


int main(int argc, char **argv) {
  char searchType;                       /* valid search types are K,C,E,B,M */
  int nextArg = 1;
  unsigned char plaintext[8];
  int i;
  char *c, buf[100];
  SEARCH_CTX ctx;
  FILE *outfile;
  char asciiBytes[] = {  0, 9, 10, 13,' ','!', '\"', '\'','(',')',',','-','.',
           '0','1','2','3','4','5','6','7','8','9',':',';','?',
           'A','B','C','D','E','F','G','H','I','J','K','L','M',
           'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','^','_','`',
           'a','b','c','d','e','f','g','h','i','j','k','l','m',
           'n','o','p','q','r','s','t','u','v','w','x','y','z' };

  printf("\nDES Search Definition Util. (Ver %s, %s). May be export "
         "controlled.\nWritten 1998 by Cryptography Research "
         "(http://www.cryptography.com) for EFF.\n"
         "This is unsupported "
         "free software: Use and distribute at your own risk.\n"
         "-------------------------------------------"
         "---------------------------------\n\n\n",
         SOFTWARE_VERSION, SOFTWARE_DATE);

  if (argc == 1) {
    printf("Parameters can be entered on the command line or entered "
           "manually.\n\nUsage modes:  (ctxt=ciphertext, ptxt=plaintext)\n"
           "    desbrute search.ctx K (8 bytes ptxt) (8 bytes ctxt)\n"
           "    desbrute search.ctx E (8 bytes ctxt0) (8 bytes ctxt1)\n"
           "    desbrute search.ctx C (8 bytes IV) (8 bytes ctxt0) "
                     "(8 bytes ctxt1)\n"
           "    desbrute search.ctx B (1 ctxt byte to repeat)\n"
           "    desbrute search.ctx M (ptxtVec) (IV) (ctxt0) (ctxt1)"
                     "(bMask) (schInf)\n\n"
           "Parameters can also be input from a file (e.g., "
           "\"desbrute < param.in\"\n\n");
  }

  /**** OPEN OUTPUT FILE ****/
  if (argc > nextArg) {
    c = argv[nextArg++];
  } else {
    printf("Enter output file for search context [ENTER=\"search.ctx\"]: ");
    gets(buf);
    if (*buf == '\0')
      strcpy(buf, "search.ctx");
    c = buf;
  }
  outfile = fopen(c, "wb");                              /* open output file */
  if (outfile == NULL)
    EXIT_ERR("Error opening output file.\n");

  /**** INITALIZE searchType ****/
  if (argc > nextArg) {
    c = argv[nextArg++];
  } else {
    printf("The array supports a variety of search types:\n");
    printf("   K - Known plaintext (standard brute force).\n");
    printf("   E - ECB ASCII text\n");
    printf("   C - CBC ASCII text\n");
    printf("   B - Blaze challenge\n");
    printf("   M - Manual parameter specification\n");
    printf("Enter search type: ");
    fgets(buf, 99, stdin);
    c = buf;
  }
  searchType = (char)toupper(c[0]);
  if (strchr("KECBM", searchType) == NULL)
    EXIT_ERR("Unknown search type.  Exiting.\n");

  /**** INITALIZE PARAMETERS FOR KNOWN PLAINTEXT SEARCHES ****/
  if (searchType == 'K') {

    /* Get known plaintext */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter known plaintext (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(plaintext, c, 8))
      EXIT_ERR("Invalid plaintext. (Must be 16 hex digits)");

    /* Get ciphertext 0 (use same for ciphertext 1) */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext0, c, 8) || unhex(ctx.ciphertext1, c, 8))
      EXIT_ERR("Invalid ciphertext. (Must be 16 hex digits.)");

    /* Set ctx */
    memset(ctx.plaintextVector, 0, sizeof(ctx.plaintextVector));
    for (i = 0; i < 8; i++)
      ctx.plaintextVector[plaintext[i]/8] |= (1 << (plaintext[i] % 8));
    ctx.plaintextByteMask = 0x00;
    memset(ctx.plaintextXorMask, 0, sizeof(ctx.plaintextXorMask));
    ctx.searchInfo = 16;            /* useCBC=0, extraXor=0, boardActiveEn=1 */
  }

  /**** INITALIZE PARAMETERS FOR ASCII SEARCHES ****/
  if (searchType == 'E' || searchType == 'C') {

    /* Get IV (only if this is ciphertext mode) */
    if (searchType == 'C') {
      if (argc > nextArg) {
        c = argv[nextArg++];
      } else {
        printf("Enter IV (16 hex digits): ");
        fgets(buf, 99, stdin);
        c = buf;
      }
      if (unhex(ctx.plaintextXorMask, c, 8))
        EXIT_ERR("Invalid IV. (Must be 16 hex digits.)");
    }

    /* Get ciphertext 0 */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext0 (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext0, c, 8))
      EXIT_ERR("Invalid ciphertext0. (Must be 16 hex digits.)");

    /* Get ciphertext 1 */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext1 (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext1, c, 8))
      EXIT_ERR("Invalid ciphertext1. (Must be 16 hex digits.)");

    /* Set ctx */
    memset(ctx.plaintextVector, 0, sizeof(ctx.plaintextVector));
    for (i = 0; i < sizeof(asciiBytes); i++)
      ctx.plaintextVector[asciiBytes[i]/8] |= (1 << (asciiBytes[i] % 8));
    ctx.plaintextByteMask = 0x00;
    if (searchType == 'E') {
      memset(ctx.plaintextXorMask, 0, sizeof(ctx.plaintextXorMask));
      ctx.searchInfo = 16;          /* useCBC=0, extraXor=0, boardActiveEn=1 */
    } else {
      /* already set plaintextXorMask = IV */
      ctx.searchInfo = 17;          /* useCBC=1, extraXor=0, boardActiveEn=1 */
    }
  }

  /**** INITALIZE PARAMETERS FOR BLAZE CHALLENGE ****/
  if (searchType == 'B') {

    /* Get ciphertext byte */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext byte (2 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext0, c, 1))
      EXIT_ERR("Invalid ciphertext byte. (Must be 2 hex digits.)");

    /* Set all ciphertext0 and ciphertext1 bytes to the input byte */
    for (i = 0; i < 8; i++)
      ctx.ciphertext0[i] = ctx.ciphertext1[i] = ctx.ciphertext0[0];

    /* Set ctx */
    memset(ctx.plaintextVector, 0, sizeof(ctx.plaintextVector));
    ctx.plaintextVector[0] = 1;                  /* halt on 00000000???????? */
    ctx.plaintextByteMask = 0x0F;                /* halt on 00000000???????? */
    memset(ctx.plaintextXorMask, 0, sizeof(ctx.plaintextXorMask));
    ctx.searchInfo = 2+16;          /* useCBC=0, extraXor=1, boardActiveEn=1 */
  }

  /**** INITALIZE PARAMETERS FOR MANUAL MODE ****/
  if (searchType == 'M') {

    /* Get plaintextVector */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("The plaintextVector specifies which bytes can appear in the\n");
      printf("plaintext.  The MSB (of the first byte entered) specifies\n");
      printf("whether 0xFF (255) can appear. The LSB is for 0x00.\n\n");
      printf("Enter plaintextVector (64 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.plaintextVector, c, 32))
      EXIT_ERR("Invalid plaintextVector. (Must be 64 hex digits.)");

    /* Get plaintextXorMask */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("The plaintextXorMask is used for the CBC mode IV.\n");
      printf("Enter plaintextXorMask (16 hex digits or ENTER=none): ");
      fgets(buf, 99, stdin);
      if (buf[0] == '\0');
        strcpy(buf, "0000000000000000");
      c = buf;
    }
    if (unhex(ctx.plaintextXorMask, c, 8))
      EXIT_ERR("Invalid plaintextXorMask. (Must be 16 hex digits.)");

    /* Get ciphertext 0 */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext0 (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext0, c, 8))
      EXIT_ERR("Invalid ciphertext0. (Must be 16 hex digits.)");

    /* Get ciphertext 1 */
    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("Enter ciphertext1 (16 hex digits): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(ctx.ciphertext1, c, 8))
      EXIT_ERR("Invalid ciphertext1. (Must be 16 hex digits.)");

    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("The plaintextByteMask specifies which bytes of the plaintext\n");
      printf("are examined in the output.  Normally this is zero, but if\n");
      printf("only partial plaintext is available, the unknown bits can\n");
      printf("be set to 1. For example, if the left-hand plaintext byte\n");
      printf("is unknown, the mask would be 0x80.\n\n");
      printf("Enter plaintextByteMask (1 byte): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(&(ctx.plaintextByteMask), c, 1))
      EXIT_ERR("Invalid plaintextByteMask. (Must be 2 hex digits.)");

    if (argc > nextArg) {
      c = argv[nextArg++];
    } else {
      printf("\n\nThe searchInfo byte has two search parameters:\n");
      printf("  bit 0x10: boardActiveEnable.  Set this to one.\n");
      printf("  bit 0x02: extraXor.  If set, after the decryption is done,\n");
      printf("                 the right half is XORed onto the left.\n");
      printf("                 This is for Matt Blaze's challenge.\n");
      printf("  bit 0x01: useCBC.  If set, the first ciphertext is XORed\n");
      printf("                 onto the second plaintext before the second\n");
      printf("                 plaintext is checked against the ");
      printf(                  "plaintextVector.\n(Higher bits control");
      printf(" searchActive, which is currently unused.)\n");
      printf("\nEnter searchInfo (1 byte): ");
      fgets(buf, 99, stdin);
      c = buf;
    }
    if (unhex(&(ctx.searchInfo), c, 1))
      EXIT_ERR("Invalid searchInfo. (Must be 2 hex digits.)");
  }

  printf("\n\n\n------------------------------ SEARCH PARAMETERS ");
  printf("------------------------------\n");
  dumpBin("  ptxtVector = ", ctx.plaintextVector, 32);
  dumpBin(" ptxtXorMask = ", ctx.plaintextXorMask, 8);
  dumpBin(" ciphertext0 = ", ctx.ciphertext0, 8);
  dumpBin(" ciphertext1 = ", ctx.ciphertext1, 8);
  dumpBin("ptxtByteMask = ", &(ctx.plaintextByteMask), 1);
  dumpBin("  searchInfo = ", &(ctx.searchInfo), 1);
  printf("--------------------------------------------");
  printf("-----------------------------------\n");

  /**** WRITE SEARCH PARAMETERS TO OUTPUT FILE ****/
  printf("\n\nWriting output file...");
  fflush(stdout);
  WriteSearchContext(outfile, &ctx);
  fclose(outfile);
  printf("Done.\n");
  return (0);
}


/*
 *  Print a descriptive string followed by a binary value (in hex)
 */
static void dumpBin(char *intro, unsigned char *data, int len) {
  int i;
  printf(intro);
  for (i=len-1; i >= 0; i--)
    printf("%02X", data[i]);
  printf("\n");
}


/*
 *  Convert an ASCII digit from hex to an int, or return -1 if not hex.
 */
static int unhexDigit(char c) {
  if (c >= '0' && c <= '9')
    return (c - '0');
  if (c >= 'a' && c <= 'f')
    return (c - 'a' + 10);
  if (c >= 'A' && c <= 'F')
    return (c - 'A' + 10);
  return (-1);                         /* return -1 for error: bad hex digit */
}


/*
 *  Convert a string of hex characters into unsigned chars.
 */
static int unhex(unsigned char *data, char *hex, int byteCount) {
  int i,j;

  if (data == NULL || hex == NULL)
    return(-1);

  /* Remove comments and whitespace */
  for (i=j=0; hex[i] != 0 && hex[i] != '%' && hex[i] != '#'; i++)
    if (hex[i] > ' ')
      hex[j++] = hex[i];
  hex[j] = '\0';

  if ((int)strlen(hex) != byteCount*2)
    return (-1);
  memset(data, 0, byteCount);
  for (i = 0; i < 2*byteCount; i++) {
    j = unhexDigit(hex[i]);
    if (j < 0)
      return (-1);
    data[byteCount - 1 - i/2] |= j << ((i & 1) ? 0 : 4);
  }
  for (i = 2*byteCount; i < (int)strlen(hex); i++)
    if (!isspace(hex[i]))
      return (-1);
  return (0);
}



