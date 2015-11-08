/*****************************************************************************
 * autoconf.c                                                                *
 *                     Search Engine Controller Program                      *
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
 *   This program automatically determines the configuration of a search     *
 *   array.  Additional diagnostic code should be added to detect common     *
 *   chip failures (once these are known).                                   *
 *                                                                           *
 *****************************************************************************
 *                                                                           *
 *   REVISION HISTORY:                                                       *
 *                                                                           *
 *   Version 1.0:  Initial release by Cryptography Research to EFF.          *
 *                 Note: Detailed diagnostic tests not implemented yet.      *
 *                                                                           *
 *****************************************************************************/

#define SOFTWARE_VERSION "1.0"
#define SOFTWARE_DATE    "04-21-1998"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
#include <ctype.h>
#include "chipio.h"

#define MAX_CHIPS_PER_BOARD  64
#define MAX_BOARDS           256

static void EXIT_ERR(char *s) { fprintf(stderr, s); exit(1); }
void AutoconfigureScan(FILE *fp, int fullScan, int verbose);
int QuickCheckRegister(int board, int chip, int register, int value);
void AddSearchUnits(FILE *fp, int board, int chip, int unit, int isGood);
long DoFullScan(FILE *fp, int board, int* chips, int verbose);


int main(int argc, char **argv) {
  int testLoops = -1;
  int baseIoPort = 0x210;
  int i, nextArg, fullScan;
  int verbose = 0;
  char buffer[200];
  char *fileSpec;
  FILE *fp;
  char *helpMsg = "Usage: autoconf search.cfg [baseIoPort] [-v] [-t#]\n\n"
                "          -v:  Verbose operation\n"
                "  search.cfg:  The output file for the config info.\n"
                "  baseIoPort:  Hex base port of I/O card (default=210 hex)\n"
                "         -t#:  Extra testing (see below)\n"
                "\nUse the -t# to do more than a quick test for chips.\n"
                "         -t0:  Do full read/write test of chip registers\n"
                "         -t#:  Do # iterations of a full system test\n";

  printf("\nDES Search Engine Configurer (Ver %s, %s). May be export "
         "controlled.\nWritten 1998 by Cryptography Research "
         "(http://www.cryptography.com) for EFF.\n"
         "This is unsupported "
         "free software: Use and distribute at your own risk.\n"
         "-------------------------------------------"
         "---------------------------------\n\n\n",
         SOFTWARE_VERSION, SOFTWARE_DATE);

  if (argc < 2 || argv[1][0] == '-')
    EXIT_ERR(helpMsg);
  fileSpec = argv[1];
  for (nextArg = 2; nextArg < argc; nextArg++) {
    if (argv[nextArg][0] == '-' || argv[nextArg][0] == '/') {
      if (toupper(argv[nextArg][1]) == 'T') {
        sscanf(argv[nextArg]+2, "%d", &testLoops);
        if (testLoops < 0)
          testLoops = 0;
      } else if (toupper(argv[nextArg][1]) == 'V')
        verbose = 1;
      else
        EXIT_ERR("Bad parameter (run with no parameters for help)\n");
    } else {
      sscanf(argv[nextArg], "%x", &baseIoPort);
      if (baseIoPort <= 0)
        EXIT_ERR("Bad parameter (run with no parameters for help)\n");
    }
  }

  if (verbose) printf("Test parameters:\n");
  if (verbose) printf("   BaseIOPort = %x\n", baseIoPort);
  if (verbose) printf("   outfile = \"%s\"\n", fileSpec);
  if (verbose) if (testLoops < 0) printf("   Quick scan only\n");
  if (verbose) if (testLoops== 0) printf("   Full register scan\n");
  if (verbose) if (testLoops > 0) printf("   %d DES tests\n", testLoops);

  fp = fopen(fileSpec, "w");
  if (fp == NULL)
    EXIT_ERR("Error opening output file.\n");
  fprintf(fp, "%% Auto-generated search system config file\n");
  fprintf(fp, "PORT=%x\n", baseIoPort);

  SetBaseAddress(baseIoPort);
  fullScan = (testLoops < 0) ? 0 : 1;
  AutoconfigureScan(fp, fullScan, verbose);
  fclose(fp);

  for (i = 0; i < testLoops; i++) {
    printf("Doing DES test %d of %d.\n", i+1, testLoops);
    fp = fopen(fileSpec, "w+");
    if (fp == NULL)
      EXIT_ERR("Error reopening output file.\n");
    fgets(buffer, 190, fp);                              /* skip header line */
    fgets(buffer, 190, fp);                                /* skip port line */
    fprintf(stderr, "*** Detailed test not implemented !!!\n");
    fclose(fp);
  }
  return (0);
}



/*
 *  Automatically figure out the configuration of the search system.
 *  Thus function assumes that SetBaseAddress() has already been called.
 */
void AutoconfigureScan(FILE *fp, int fullScan, int verbose) {
  int board, chip, chipCount, value;
  long totalChips = 0;
  int chips[MAX_CHIPS_PER_BOARD];

  if (verbose) printf("**** DOING AUTOCONFIGURE SCAN ****\n");
  for (board = 0; board < MAX_BOARDS; board++) {
    printf("CHECKING BOARD 0x%02X: ", board);
    fflush(stdout);
    chipCount = 0;
    for (chip = 0; chip < MAX_CHIPS_PER_BOARD; chip++) {
      /* TEST FIRST BYTE OF CIPHERTEXT 0 (REGISTER 0x28) */
      value = rand() & 0xFF;
      if (QuickCheckRegister(board, chip, 0x28, value) == 0 ||
          QuickCheckRegister(board, chip, 0x28, value^255) == 0) {
        chips[chip] = 0;
        if (verbose) printf("\n   BOARD 0x%02X CHIP 0x%02X: Not found.",
                board, chip);
      } else {
        chips[chip] = 1;
        chipCount++;
        if (verbose) printf("\n   BOARD 0x%02X CHIP 0x%02X: FOUND",board,chip);
        if (fullScan) {
          if (verbose) printf("\n   CHIP 0x%02X: Halting chip for test", chip);
          SetRegister(board, chip, REG_PTXT_BYTE_MASK, 0xFF);
        }
      }
    }
    if (verbose) printf("\n");
    printf("  Found %4d chips total.\n", chipCount);

    /* DO DETAILED REGISTER SCAN IF REQUESTED */
    if (fullScan && chipCount) {
      totalChips = DoFullScan(fp, board, chips, verbose);
    } else {
      chipCount = 0;
      for (chip = 0; chip < MAX_CHIPS_PER_BOARD; chip++) {
        if (chips[chip]) {
          chipCount++;
          totalChips++;
          AddSearchUnits(fp, board, chip, -1, 1);
        }
      }
    }
  }
  if (verbose) printf("*** AUTOCONFIGURE SCAN COMPLETE ***\n");
  printf("Found %ld chips total.\n", totalChips);
}


int QuickCheckRegister(int board, int chip, int reg, int value) {
  SetRegister(board, chip, reg, value);
  if (GetRegister(board, chip, reg) != value)
    return (0);
  return (1);
}


void AddSearchUnits(FILE *fp, int board, int chip, int unit, int isGood) {
  int i;

  if (unit < 0) {
    for (i = 0; i < SEARCH_UNITS_PER_CHIP; i++)
      AddSearchUnits(fp, board, chip, i, 1);
  } else {
    fprintf(fp, "%s=0x%02X 0x%02X 0x%02X\n", isGood ? "UNIT" : "FAIL",
            board, chip, unit);
  }
}


long DoFullScan(FILE *fp, int board, int* chips, int verbose) {
  int chip, reg, seed, value, i, j;
  int units[24];
  long totalChips = 0;

  if (verbose) printf("   --- Register scan on board 0x%02X ---\n", board);

  /* PICK A SEED & USE IT TWICE (ONCE WHEN SETTING & ONCE WHEN CHECKING */
  seed = (int)time(NULL);

  /*** SET REGISTERS ***/
  srand(seed);
  for (chip = 0; chip < MAX_CHIPS_PER_BOARD; chip++) {
    if (chips[chip] == 0)
      continue;
    if (verbose) printf("   BOARD 0x%02X CHIP 0x%02X: Setting regs.\n",
            board, chip);
    for (reg = 0; reg <= 0xFF; reg++) {
      if ((reg >= 0x39 && reg < 0x40) || (reg > 0x40 && (reg & 7) == 7))
            continue;
        value = rand() & 255;
        SetRegister(board, chip, reg, value);
    }
  }

  /*** CHECK REGISTERS ***/
  srand(seed);
  for (chip = 0; chip < MAX_CHIPS_PER_BOARD; chip++) {
    if (chips[chip] == 0)
      continue;
    for (i = 0; i < 24; i++)
      units[i] = 1;
    if (verbose) printf("   BOARD 0x%02X CHIP 0x%02X: Checking...\n",
            board, chip);
    for (reg = 0; reg <= 0xFF; reg++) {
      if ((reg >= 0x39 && reg < 0x40) || (reg > 0x40 && (reg & 7) == 7))
            continue;
      value = rand() & 255;
      i = GetRegister(board, chip, reg);
      SetRegister(board, chip, reg, value ^ 255);
      j = GetRegister(board, chip, reg);
      if (i != value || j != (value ^ 255)) {
        if (chips[chip])
          printf("\n *** BOARD 0x%02X, CHIP 0x%02X FAILED ***\n  Details: ",
                  board, chip);
        if (reg < 0x40)
          chips[chip] = 0;
        else
          units[(reg - 0x40)/8] = 0;
        if (i != value || j != value)
          printf("\n  Board 0x%02X Chip 0x%02X Reg 0x%02X bad:",
            board, chip, reg);
        if (i != value)
          printf("  Got 0x%02X, not %02X.", i, value);
        if (j != (value ^ 255))
          printf("  Got 0x%02X, not %02X.", j, value ^ 255);
      } else {
        if (verbose)
          printf("\n   Reg 0x%02X good (Read 0x%02X)", reg, value);
      }
    }
    if (chips[chip] == 0)
      printf("\n  -- CHIP FAILED --\n");
    else {
      for (i = 0; i < 24; i++)
        AddSearchUnits(fp, board, chip, i, units[i]);
      totalChips++;
    }
  }
  return (totalChips);
}


