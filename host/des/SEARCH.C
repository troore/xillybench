/*****************************************************************************
 * search.c                                                                  *
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
 *   REVISION HISTORY:                                                       *
 *                                                                           *
 *   Version 1.0:  Initial release by Cryptography Research to EFF.          *
 *                                                                           *
 *****************************************************************************/

#define SOFTWARE_VERSION "1.0"
#define SOFTWARE_DATE    "04-21-1998"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <memory.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "search.h"
#include "chipio.h"
#include "keyblock.h"
#include "des.h"



/*
 *  SEARCH_CHIP STRUCTURE: Contains status information about each chip.
 *
 *    board:  The board this chip is on (1 byte).
 *    chip:  The ID of this chip on the board (1 byte).
 *    initialized:  0=uninitialized, 1=initialized, -1=defective.
 *    region[]: Specifies the top 24 bits of the key being searched by each
 *        search unit. A value of -1 means the search unit is idle
 *        (idle), and a value of -2 means the search unit is not used.
 *    overFlow[]: Specifies the value at which the low 32 bits of the
 *        key (the key counter) will have gone through all 2^32
 *        possibilities.  Note: this only has the top 24 bits of the
 *        counter, which corresponds to key bytes: .. .. .. XX XX XX.. (LSB)
 *    lastSeen[]:  The value last seen in the low 32 bits of the key.
 *        This has the same encoding as overFlow.
 */
typedef struct CHIP_CTX {
  unsigned char board, chip;
  int initialized;
  long region[SEARCH_UNITS_PER_CHIP];
  long overFlow[SEARCH_UNITS_PER_CHIP];
  long lastDone[SEARCH_UNITS_PER_CHIP];
  struct CHIP_CTX *nextChip;
} CHIP_CTX;


/*
 *  GLOBAL VARIABLES
 */
CHIP_CTX *CHIP_ARRAY = NULL;
SEARCH_CTX CTX;
static int QUIET = 0;
static int VERBOSE = 0;
static FILE *FP_LOG = NULL;


/*
 *  FUNCTION PROTOTYPES & MINI FUNCTIONS & MACROS
 */
static void EXIT_ERR(char *s) { fprintf(stderr, s); exit(1); }
long ReadConfig(char *configFilespec);
void RunSearch(FILE *ctxFile);
void InitializeChip(CHIP_CTX *cp, SEARCH_CTX *ctx);
void ServiceChip(CHIP_CTX *cp, SEARCH_CTX *ctx, FILE *ctxFile);
long GetUnitKeyCounter(int board, int chip, int unit);
void CheckAndPrintKey(CHIP_CTX *cp, SEARCH_CTX *ctx, int unit);
int ServiceKeyboard(SEARCH_CTX *ctx);
int CheckKey(unsigned char key[56], SEARCH_CTX *ctx);


/*
 *  ReadConfig():  Read the search array configuration file.  This file
 *      specifies the I/O base port for SetBaseAddress and also the
 *      search units.  It can contain 3 kinds of lines: comments that
 *      that with '%', base port with "PORT=210" for port 210 hex, and
 *      "UNIT= 12 32 8" to add a search unit on board 0x12, chip 0x32,
 *      and unit 0x08 (all hex).  The function constructs CHIP_ARRAY
 *      as a linked list of chips.
 *   Returns: Total number of search units.
 */
long ReadConfig(char *configFilespec) {
  char buffer[200];
  int basePort = -1;
  int board, chip, unit, i;
  int lastBoard = -1, lastChip = -1;
  long totalUnits = 0;
  CHIP_CTX *cp;
  FILE *fp;

  cp = CHIP_ARRAY;
  if (cp != NULL)
    EXIT_ERR("Chip array base isn't NULL. (Internal error.)\n");

  fp = fopen(configFilespec, "rb");
  if (fp == NULL)
    EXIT_ERR("Error opening configuration filespec.\n");

  if (!QUIET) printf("Reading configuration file \"%s\".\n", configFilespec);
  while (fgets (buffer, 190, fp) != NULL) {
    if (buffer[0] == '\0' || buffer[0] == '%')
      continue;
    if (memcmp(buffer, "PORT=", 5) == 0) {
      basePort = 0;
      sscanf(buffer+5, "%x", &basePort);
      if (basePort <= 0)
        EXIT_ERR("Defective PORT= in configuration file.\n");
      SetBaseAddress(basePort);
      if (!QUIET) printf("Set base port to %x\n", basePort);
      if (FP_LOG && VERBOSE) fprintf(FP_LOG, "Set base port=0x%x\n", basePort);
    }
    else if (memcmp(buffer, "UNIT=", 5) == 0 ||
            memcmp(buffer, "FAIL=", 5) == 0) {
      board = chip = unit = -1;
      sscanf(buffer+5, "%x %x %x", &board, &chip, &unit);
      if (board < 0 || chip < 0 || unit < 0)
        EXIT_ERR("Defective UNIT= or FAIL= in configuration file.\n");
      if (board < lastBoard || (board == lastBoard && chip < lastChip))
        EXIT_ERR("Bad UNIT= or FAIL= in config: board & chip must decrease\n");
      if (board != lastBoard || chip != lastChip) {
        lastBoard = board;
        lastChip = chip;
        if (cp == NULL)
          cp = CHIP_ARRAY = malloc(sizeof(CHIP_CTX));
        else {
          cp->nextChip = malloc(sizeof(CHIP_CTX));
          cp = cp->nextChip;
        }
        cp->board = (unsigned char)board;
        cp->chip = (unsigned char)chip;
        cp->initialized = 0;
        for (i = 0; i < SEARCH_UNITS_PER_CHIP; i++)
          cp->region[i] = -2;
        cp->nextChip = NULL;
      }
      if (cp->region[unit] == -2 && memcmp(buffer, "UNIT=", 5) == 0) {
        totalUnits++;
        cp->region[unit] = -1;                    /* mark the unit as extant */
      }
    } else {
      fprintf(stderr, "IGNORING UNKNOWN CONFIG FILE LINE: \"%s\"\n", buffer);
    }
  }
  fclose(fp);
  if (CHIP_ARRAY == NULL)
    EXIT_ERR("Error: Configuration file does not have any valid units.\n");

  if (FP_LOG && VERBOSE) {
    fprintf(FP_LOG, "Configuration summary:\n");
    for (cp = CHIP_ARRAY; cp != NULL; cp = cp->nextChip) {
      for (i = 0; i < SEARCH_UNITS_PER_CHIP; i++)
        if (cp->region[i] != -2)
          fprintf(FP_LOG, "%s=0x%02X 0x%02X 0x%02X\n",
                  (cp->initialized >= 0) ? "UNIT" : "FAIL",
                   cp->board, cp->chip, i);
    }
  }
  if (!QUIET) printf("Config done: Found %ld search units.\n", totalUnits);
  if (FP_LOG) fprintf(FP_LOG, "Config found %ld search units.\n", totalUnits);
  return (totalUnits);
}



void main(int argc, char **argv) {
  FILE *ctxFile;
  int i;
  time_t t;
  CHIP_CTX *cp;

  printf("\nDES Search Engine Controller (Ver %s, %s). May be export "
         "controlled.\nWritten 1998 by Cryptography Research "
         "(http://www.cryptography.com) for EFF.\n"
         "This is unsupported "
         "free software: Use and distribute at your own risk.\n"
         "-------------------------------------------"
         "---------------------------------\n\n\n",
         SOFTWARE_VERSION, SOFTWARE_DATE);
  if (argc < 3) {
    fprintf(stderr,
         "Usage:  search configFile contextFile [logfile] [-v] [-q]\n"
         "     configFile: Search array configuration from autoconf\n"
         "    contextFile: Search context (from init)\n"
         "        logfile: Output file with detailed reporting info\n"
         "             -v: verbose output to logfile\n"
         "             -q: quiet mode (less output to the screen)\n"
         "  (Note: paramaters must be in the order above.)\n");
    exit(1);
  }
  for (i = 3; i < argc; i++) {
    if (i == 3 && argv[i][0] != '-') {
      FP_LOG = fopen(argv[3], "w");
      if (FP_LOG == NULL)
        EXIT_ERR("Error opening log file.");
    } else if (stricmp(argv[i], "-v") == 0)
      VERBOSE = 1;
    else if (stricmp(argv[i], "-q") == 0)
      QUIET = 1;
    else {
      fprintf(stderr, "Unknown parameter \"%s\"\n", argv[i]);
      exit(1);
    }
  }

  /* READ CONFIGURATION FILE SPECIFYING BASE PORT AND SEARCH UNITS */
  CTX.totalUnits = ReadConfig(argv[1]);

  /* RESET THE SEARCH ARRAY */
  if (!QUIET) printf("Resetting the search array.\n");
  i = -1;
  for (cp = CHIP_ARRAY; cp != NULL; cp = cp->nextChip) {
    if (i != cp->board) {
      i = cp->board;
      ResetBoard(i);
    }
  }
  t = time(NULL);

  /* READ SEARCH FILE SPECIFYING SEARCH INFO & REMAINING KEY BLOCKS */
  ctxFile = fopen(argv[2], "r+b");
  if (ctxFile == NULL) {
    fprintf(stderr, "Error opening search context file \"%s\"\n", argv[2]);
    exit(1);
  }

  /* MAKE SURE RESET HAD AT LEAST 1 SECOND TO SETTLE. */
  if (!QUIET) printf("Waiting for reset to settle.\n");
  while(t + 1 >= time(NULL)) {}

  /* RUN THE SEARCH! */
  RunSearch(ctxFile);
  fclose(ctxFile);
  if (!QUIET) printf("Exiting.\n");
}


/*
 *  Run the search. Uses the search parameters in the
 *       global linked list CHIP_ARRAY and keeps its context info
 *       in the global CTX.
 */
void RunSearch(FILE *ctxFile) {
  CHIP_CTX *cp;
  SEARCH_CTX *ctx = &CTX;
  int halt = 0;
  time_t startTime, lastReportTime, t;
  long loopCount = 0;
  char buffer[128];

  if (!QUIET) printf("Loading search context file...\n");
  OpenSearchContext(ctxFile, ctx);

  printf("Initialization Successful - Beginning search.\n");
  if (QUIET) printf("Quiet mode: Press ? for help during search.\n");
  if (FP_LOG && VERBOSE) fprintf(FP_LOG, "--- Beginning search ---\n");
  for (cp = CHIP_ARRAY; cp != NULL; cp = cp->nextChip)
    InitializeChip(cp, ctx);
  startTime = time(NULL);
  lastReportTime = 0;

  while (halt == 0) {
    t = time(NULL);                                /* report every 5 seconds */
    if (t/5 != lastReportTime/5) {
      sprintf(buffer, "%7ld blocks done, %7ld left, %4ld running (time=%7ld).",
              ctx->totalFinishedKeyBlocks, ctx->totalUnstartedKeyBlocks +
              ctx->totalPendingKeyBlocks, ctx->totalPendingKeyBlocks,
              (long)(t - startTime));
      if (!QUIET) printf(">%s ('?'=help)\n", buffer);
      if (FP_LOG && VERBOSE) fprintf(FP_LOG, "Report: %s\n", buffer);
      lastReportTime = t;
    }
    for (cp = CHIP_ARRAY; cp != NULL && halt == 0; cp = cp->nextChip) {
      ServiceChip(cp, ctx, ctxFile);
      if (ServiceKeyboard(ctx) < 0)
        halt = 1;
    }
    if (ctx->totalFinishedKeyBlocks == (1L<<24))
      halt = 1;
    GetRegister(255, 255, 255);
    loopCount++;
  }
}


/*
 *  InitializeChip(cp, ctx):  Initialize a chip whose chip context is
 *      at cp, using the search parameters at ctx.
 */
void InitializeChip(CHIP_CTX *cp, SEARCH_CTX *ctx) {
  int i,j;

  if (!QUIET) printf("Initializing board 0x%02X, chip 0x%02X\n",
          cp->board, cp->chip);
  if (FP_LOG && VERBOSE) fprintf(FP_LOG,
          "Initializing board 0x%02X, chip 0x%02X\n", cp->board, cp->chip);
  SetRegister(cp->board, cp->chip, REG_PTXT_BYTE_MASK, 0xFF);   /* halt chip */
  for (i = 0; i < 32; i++)
    SetRegister(cp->board, cp->chip, REG_PTXT_VECTOR+i,
            ctx->plaintextVector[i]);
  for (i = 0; i < 8; i++)
    SetRegister(cp->board, cp->chip,REG_PTXT_XOR_MASK+i,
            ctx->plaintextXorMask[i]);
  for (i = 0; i < 8; i++)
    SetRegister(cp->board, cp->chip, REG_CIPHERTEXT0+i, ctx->ciphertext0[i]);
  for (i = 0; i < 8; i++)
    SetRegister(cp->board, cp->chip, REG_CIPHERTEXT1+i, ctx->ciphertext1[i]);
  SetRegister(cp->board, cp->chip, REG_PTXT_BYTE_MASK, ctx->plaintextByteMask);
  SetRegister(cp->board, cp->chip, REG_SEARCHINFO, ctx->searchInfo);

  /* TO BE SAFE, VERIFY THAT ALL REGISTERS WERE WRITTEN PROPERLY */
  /* (Each chip only gets initialized once, so this is quick.) */
  j = 0;
  for (i = 0; i < 32; i++)
    j += CheckRegister(cp->board, cp->chip, REG_PTXT_VECTOR+i,
            ctx->plaintextVector[i]);
  for (i = 0; i < 8; i++) {
    j += CheckRegister(cp->board, cp->chip, REG_PTXT_XOR_MASK+i,
            ctx->plaintextXorMask[i]);
    j += CheckRegister(cp->board, cp->chip, REG_CIPHERTEXT0+i,
            ctx->ciphertext0[i]);
    j += CheckRegister(cp->board, cp->chip, REG_CIPHERTEXT1+i,
            ctx->ciphertext1[i]);
  }
  j += CheckRegister(cp->board, cp->chip, REG_PTXT_BYTE_MASK,
          ctx->plaintextByteMask);
  j += CheckRegister(cp->board, cp->chip, REG_SEARCHINFO, ctx->searchInfo);
  if (j != 0) {
    printf("Bad register on board 0x%02X, chip 0x%02X. Chip disabled.\n",
            cp->board, cp->chip);
    if (FP_LOG) fprintf(FP_LOG, "Bad register on board 0x%02X, chip 0x%02X.%s",
            cp->board, cp->chip, " Chip disabled.\n");
  }

  /* UPDATE THE CHIP CONTEXT */
  cp->initialized = (j == 0) ? 1 : -1;           /* initialized or defective */
}


/*
 *  Service a chip by doing the following:
 *    - Check if it has halted
 *    - Check to see if it has finished its region
 *    - Restart if it is idle
 */
void ServiceChip(CHIP_CTX *cp, SEARCH_CTX *ctx, FILE *ctxFile) {
  int unit;
  long k;

  if (cp->initialized < 0)
    return;

  /*
   *  READ KEYS & RESTART ANY HALTED UNITS
   */
  for (unit = 0; unit < SEARCH_UNITS_PER_CHIP; unit++) {
    if (cp->region[unit] >= 0) {                     /* if currently running */
      if (!(GetRegister(cp->board, cp->chip, REG_SEARCH_STATUS(unit)) & 1)) {
        CheckAndPrintKey(cp, ctx, unit);
        SetRegister(cp->board, cp->chip, REG_SEARCH_STATUS(unit), 1);
      }
    }
  }

  /*
   *  See if any units have completed their search regions
   *    Note: If I/O bandwidth was a problem and the clock rate of the
   *       search system was fixed, we could predict when the keycounter
   *       would flip and avoid this check.
   */
  for (unit = 0; unit < SEARCH_UNITS_PER_CHIP; unit++) {
    if (cp->region[unit] < 0)
      continue;
    k = GetUnitKeyCounter(cp->board, cp->chip, unit);
    k -= cp->overFlow[unit];
    if (k < 0)
      k += (1L << 24);
    if (VERBOSE && FP_LOG) fprintf(FP_LOG,
              "Board 0x%02X chip 0x%02X unit 0x%02X is at 0x%06lX "
              "(lastDone=0x%06lX, overFlow=%06lX)\n",
              cp->board, cp->chip, unit, k,
              cp->lastDone[unit], cp->overFlow[unit]);
    if (k < cp->lastDone[unit]) {
      if (!QUIET) printf("Board 0x%02X chip 0x%02X unit 0x%02X finished block "
              "0x%06lX (lastDone=0x%06lX, got 0x%06lX, overFlow=%06lX)\n",
              cp->board, cp->chip, unit, cp->region[unit],
              cp->lastDone[unit], k, cp->overFlow[unit]);
      if (FP_LOG) fprintf(FP_LOG, "Unit 0x%02X 0x%02X 0x%02X finished "
              "0x%06lX (last=%06lX, got %06lX, oFlow=%06lX)\n",
              cp->board, cp->chip, unit, cp->region[unit],
              cp->lastDone[unit], k, cp->overFlow[unit]);
      FinishKeyRegion(ctxFile, ctx, cp->region[unit]);     /* region is done */
      cp->region[unit] = -1;                             /* unit is now idle */
    } else {
      cp->lastDone[unit] = k;
    }
  }

  /*
   *  Start any units that are currently stalled
   */
  for (unit = 0; unit < SEARCH_UNITS_PER_CHIP; unit++) {
    if (cp->region[unit] == -1) {
      k = ReserveKeyRegion(ctxFile, ctx);
      if (k < 0)
        break;                                         /* no more regions... */
      if (!QUIET) printf("Starting board 0x%02X, chip 0x%02X, unit 0x%02X... ",
           cp->board, cp->chip, unit);
      if (FP_LOG) fprintf(FP_LOG, "Starting unit 0x%02X 0x%02X 0x%02X... ",
           cp->board, cp->chip, unit);
      cp->region[unit] = k;

      /* LOAD UP THE KEY REGION AND LET 'ER RIP... */
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+6,
              (unsigned char)((k >> 16) & 0xFF));
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+5,
              (unsigned char)((k >> 8) & 0xFF));
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+4,
              (unsigned char)(k & 0xFF));
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+3, 0);
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+2, 0);
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+1, 0);
      SetRegister(cp->board, cp->chip, REG_SEARCH_KEY(unit)+0, 0);

      SetRegister(cp->board, cp->chip, REG_SEARCH_STATUS(unit), 1);   /* GO! */

      /* READ OUT THE KEY COUNTER (3 BYTES) FOR OVERFLOW SENSING */
      k = GetUnitKeyCounter(cp->board, cp->chip, unit);
      cp->overFlow[unit] = k;
      cp->lastDone[unit] = k;
      if (!QUIET) printf("Region=0x%06lX, overFlow=0x%06lX\n",
            cp->region[unit], k);
      if (FP_LOG) fprintf(FP_LOG, "Region=0x%06lX, overFlow=0x%06lX\n",
            cp->region[unit], k);
    }
  }
}


/*
 *  Read the value of a rapidly-incrementing key counter register.
 *     The function reads the register twice, finds the most-significant
 *     bit that changed during the operation, and returns the later
 *     (higher) value with all bits to the right of the one that changed
 *     set to zero.
 *  The return value is the top 24 bits of the low 32 bits of the
 *     key counter -- i.e., key bytes (MSB).. .. .. XX XX XX ..(LSB)
 */
long GetUnitKeyCounter(int board, int chip, int unit) {
  long v1, v2, m;
  do {
    v1 = ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+3)) << 16;
    v1 |= ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+2)) << 8;
    v1 |= ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+1));
    v2 = ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+3)) << 16;
    v2 |= ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+2)) << 8;
    v2 |= ((long)GetRegister(board, chip, REG_SEARCH_KEY(unit)+1));
  } while (v1 > v2);
  for (m = 0x800000L; m != 0; m >>= 1) {
    if ((v1 & m) != (v2 & m)) {
      v2 = (v2 & (0xFFFFFFL - m + 1));
      break;
    }
  }
  return (v2);
}


/*
 *  Get the key out of a halted unit and print it to the screen/logs
 */
void CheckAndPrintKey(CHIP_CTX *cp, SEARCH_CTX *ctx, int unit) {
  unsigned char key[7];
  unsigned char binKey[56];
  char buf[128];
  int i,j, goodKey;

  for (i = 0; i < 7; i++)
    key[i] = (unsigned char)GetRegister(cp->board, cp->chip,
            REG_SEARCH_KEY(unit)+i);
  if (--(key[0]) == 0xFF)                                   /* Decrement key */
    if (--(key[1]) == 0xFF)
      if (--(key[2]) == 0xFF)
        --key[3];
  for (i = 0; i < 56; i++)
    binKey[i] = (key[i/8] >> (i&7)) & 1;
  for (i = 7; i >= 0; i--) {
    j = binKey[i*7]*2 + binKey[i*7+1]*4 + binKey[i*7+2]*8 + binKey[i*7+3]*16 +
        binKey[i*7+4]*32 + binKey[i*7+5]*64 + binKey[i*7+6]*128;
    sprintf(buf+14-2*i, "%02X", j);
  }

  if (QUIET)
    printf("Halt in %02X.%02X.%02X, K=%s P=", cp->board, cp->chip, unit, buf);
  else {
    printf("BOARD 0x%02X, CHIP 0x%02X, UNIT 0x%02X HALTED!\n    K56 = ",
            cp->board, cp->chip, unit);
    for (i = 6; i >= 0; i--) printf("%02X", key[i]);
    printf("\n    K64 = %s\n", buf);
  }
  if (FP_LOG) {
    fprintf(FP_LOG, "Halt@ %02X.%02X.%02X, K=",
            cp->board, cp->chip, unit);
    for (i = 6; i >= 0; i--) fprintf(FP_LOG, "%02X", key[i]);
    if (VERBOSE) fprintf(FP_LOG, ", K64=%s", buf);
  }

  goodKey = CheckKey(binKey, ctx);                      /* prints plaintexts */

  if (QUIET) printf(goodKey ? " (OK!)\n" : " (BAD)\n");
  else printf("    ***** KEY IS %s *****\n", goodKey ? " OKAY " : "BAD");
  if (FP_LOG) fprintf(FP_LOG, goodKey ? " (=OK!)\n" : " (=BAD)\n");
  fflush(stdout);
  if (FP_LOG) fflush(FP_LOG);
}


/*
 *  Let the user see what's going on.
 */
int ServiceKeyboard(SEARCH_CTX *ctx) {
  int k, i, board, chip, reg, val;
  char buffer[128];

  while (kbhit()) {
    k = toupper(getch());
    if (k == '?') {
      printf("Keystroke options:\n    ESC=quit search\n");
      printf("    R=read a chip\n    SPACE=status\n    P=pause\n");
      printf("    S=set register\n");
      printf("Press a command letter, ENTER to continue\n");
      while (!kbhit()) {}
      continue;
    }
    if (k == 'P') {
      fprintf(stderr, "  --- PAUSED ---\n(Press a command letter, ");
      fprintf(stderr, "ENTER to continue, or ? for help.)\n");
      while (!kbhit()) {}
      continue;
    }
    if (k == 27) {
      fprintf(stderr, "  -- ESC PRESSED! HIT 'Y' TO CONFIRM HALT --\n");
      if (toupper(getch()) == 'Y') {
        fprintf(stderr, "Halting...\n");
        return (-1);
      }
      fprintf(stderr, "   (Not halting.)\n");
      continue;
    }
    if (k == ' ') {
      fprintf(stderr, "There are %ld search units running\n", ctx->totalUnits);
      fprintf(stderr, "Of %ld blocks: %ld done, %ld unstarted, %ld pending\n",
            1L<<24, ctx->totalFinishedKeyBlocks, ctx->totalUnstartedKeyBlocks,
            ctx->totalPendingKeyBlocks);
      fprintf(stderr, "The next key block to start is 0x%06lX.\n",
              ctx->nextUnstartedKeyBlock);
      fprintf(stderr, "Press a command letter or ENTER to continue\n");
      while (!kbhit()) {}
    }
    if (k == 'R') {
      fprintf(stderr, "Enter board and chip (in hex): ");
      fgets(buffer, 127, stdin);
      board = chip = -1;
      sscanf(buffer, "%x %x", &board, &chip);
      if (board < 0 || board > 255 || chip < 0 || chip > 255) {
        fprintf(stderr, "Bad board (0x%02X) or chip (0x%02X)\n", board, chip);
        continue;
      }
      for (i = 0; i < 256; i++) {
        if ((i & 15) == 0)
          printf("\n0x%02X 0x%02X 0x%02X:", board, chip, i);
        printf(" %02X", GetRegister(board, chip, i));
      }
      printf("\n");
      fprintf(stderr, "Press a command letter or ENTER to continue\n");
      while (!kbhit()) {}
      continue;
    }
    if (k == 'S') {
      fprintf(stderr, "Enter board chip reg value (all hex): ");
      fgets(buffer, 127, stdin);
      board = chip = reg = val = -1;
      sscanf(buffer, "%x %x %x %x", &board, &chip, &reg, &val);
      if (board >= 0 && chip >= 0 && reg >= 0 && val >= 0) {
        fprintf(stderr, "Writing 0x%02X to 0x%02X.0x%02X reg 0x%02X\n",
            val, board, chip, reg);
        SetRegister(board, chip, reg, val);
      }
      fprintf(stderr, "Press a command letter or ENTER to continue.\n");
      while (!kbhit()) {}
      continue;
    }
  }
  return (0);
}


/*
 *  If needed, this function can be used to decide whether keys are
 *      actually good or not to reject false positives.
 *  Returns 1 if the key is not bad, zero if it is wrong.
 */
int CheckKey(unsigned char key[56], SEARCH_CTX *ctx) {
  bool ctxt[64],ptxt0[64],ptxt1[64];
  unsigned char p0[8],p1[8];
  int i,c;

  /* Compute the plaintext and try to print it to the screen */
  for (i = 0; i < 64; i++)
    ctxt[i] = (ctx->ciphertext0[i/8] >> (i&7)) & 1;
  DecryptDES((bool*)key, ptxt0, ctxt, 0);
  for (i = 0; i < 8; i++) {
    p0[i] = (unsigned char)(ptxt0[i*8+0]+ptxt0[i*8+1]*2+ptxt0[i*8+2]*4+
           ptxt0[i*8+3]*8+ptxt0[i*8+4]*16+ptxt0[i*8+5]*32+ptxt0[i*8+6]*64+
           ptxt0[i*8+7]*128);
  }
  for (i = 0; i < 8; i++)
    p0[i] ^= ctx->plaintextXorMask[i];
  if (!QUIET) {
    printf("    Plaintext0 =");
    for (i = 7; i>=0; i--) printf(" %02X", p0[i]);
    printf("   (\"");
    for (i = 7; i>=0; i--)
      printf("%c", (p0[i] < 32) ? '?' : p0[i]);
    printf("\")\n");
  }
  if (QUIET) for (i = 7; i>=0; i--) printf("%02X", p0[i]);
  if (FP_LOG) fprintf(FP_LOG, ", ptxt=");
  if (FP_LOG) for (i = 7; i>=0; i--) fprintf(FP_LOG, "%02X", p0[i]);

  for (i = 0; i < 64; i++)
    ctxt[i] = (ctx->ciphertext1[i/8] >> (i&7)) & 1;
  DecryptDES((bool*)key, ptxt1, ctxt, 0);
  for (i = 0; i < 8; i++) {
    p1[i] = (unsigned char)(ptxt1[i*8+0]+ptxt1[i*8+1]*2+ptxt1[i*8+2]*4+
           ptxt1[i*8+3]*8+ptxt1[i*8+4]*16+ptxt1[i*8+5]*32+ptxt1[i*8+6]*64+
           ptxt1[i*8+7]*128);
  }
  if (ctx->searchInfo & 1) {                 /* if CBC mode, XOR w/ 1st ctxt */
    for (i = 0; i < 8; i++)
      p1[i] ^= ctx->ciphertext0[i];
  }
  if (!QUIET) printf("    Plaintext1 =");
  if (QUIET) printf("/");
  if (FP_LOG) fprintf(FP_LOG, "/");
  if (!QUIET) for (i = 7; i>=0; i--) printf(" %02X", p1[i]);
  if (QUIET) for (i = 7; i>=0; i--) printf("%02X", p1[i]);
  if (FP_LOG) for (i = 7; i>=0; i--) fprintf(FP_LOG, "%02X", p1[i]);
  if (!QUIET) {
    printf("   (\"");
    for (i = 7; i>=0; i--)
      printf("%c", (p1[i] < 32) ? '?' : p1[i]);
    printf("\")\n");
  }

  /* Reject key if doesn't contain good characters */
  for(i = 0; i < 8;i++) {
    if (((ctx->plaintextByteMask) >> i) & 1)
      continue;
    c = p0[i];
    if (((ctx->plaintextVector[c/8] >> (c & 7)) & 1) == 0)
      return (0);
    c = p1[i];
    if (((ctx->plaintextVector[c/8] >> (c & 7)) & 1) == 0)
      return (0);
  }

  /*
   *  INSERT ADDITIONAL CODE HERE TO REJECT FALSE POSITIVES
   */
  return (1);
}

