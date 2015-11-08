/*****************************************************************************
 * chipio.c                                                                  *
 *             Search Engine Low-Level Hardware Interface Module             *
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
#include <conio.h>
#include <stdlib.h>
#include "chipio.h"

static int CURRENT_BOARD     = -1;
static int CURRENT_CHIP      = -1;
static int CURRENT_PORT_CNFG = -1;
static int IO_BASE_ADDRESS = 0x210;

#define IO_PORTA_ADDRESS (IO_BASE_ADDRESS+0)
#define IO_PORTB_ADDRESS (IO_BASE_ADDRESS+1)
#define IO_PORTC_ADDRESS (IO_BASE_ADDRESS+2)
#define IO_CNFG_ADDRESS  (IO_BASE_ADDRESS+3)
#define CNFG_OUTPUT   0x80
#define CNFG_INPUT    0x82

#define CTRL_BASE        0x1B      /* base value onto which others are XORed */
#define CTRL_RST         0x20
#define CTRL_RDB         0x10
#define CTRL_WRB         0x08
#define CTRL_ALE         0x04
#define CTRL_ADRSEL2     0x02       /* in documentation is also called CNTR1 */
#define CTRL_ADRSEL1     0x01       /* in documentation is also called CNTR0 */

/*
 *  DELAYS CAN BE ADDED TO DEAL WITH BUS LOADING/CAPACITANCE/ETC.
 */
#define DELAY_FACTOR 100L
#define DELAY_ADDRESS_SETTLE   0*DELAY_FACTOR
#define DELAY_DATA_SETTLE      0*DELAY_FACTOR
#define DELAY_RST_HOLD         0*DELAY_FACTOR
#define DELAY_RST_RECOVER      0*DELAY_FACTOR
#define DELAY_RDB_HOLD         0*DELAY_FACTOR
#define DELAY_RDB_RECOVER      0*DELAY_FACTOR
#define DELAY_WRB_HOLD         0*DELAY_FACTOR
#define DELAY_WRB_RECOVER      0*DELAY_FACTOR
#define DELAY_ALE_SETTLE       0*DELAY_FACTOR
#define DELAY_ADRSEL2_SETTLE   0*DELAY_FACTOR
#define DELAY_ADRSEL1_SETTLE   0*DELAY_FACTOR

#define ioDelay(delayTime)     {}                     /* insert delay if rqd */


#ifdef _MSC_VER
/*
 *  Microsoft C++ Direct I/O Functions
 */
static int inportb(int portNum) {
  unsigned char rval;
  unsigned short portNumShort = (unsigned short)portNum;

  _asm { mov dx,portNumShort }
  _asm { in al,dx }
  _asm { mov rval, al }
  return (rval);
}

static void outportb(int portNum, int val) {
  unsigned char valChar = (unsigned char)val;
  unsigned short portNumShort = (unsigned short)portNum;

  _asm { mov dx, portNumShort }
  _asm { mov al, valChar }
  _asm { out dx, al }
}
#endif


static void ConfigureIO_Port(int inputOrOutput) {
  outportb(IO_CNFG_ADDRESS, inputOrOutput);
  CURRENT_PORT_CNFG = inputOrOutput;

  /*  Warning:
   *
   *  Changing the IO port state causes a tiny glitch to go out on the
   *  PC-DIO card.  This is enough to ocasionally trigger the ALE, which
   *  causes read/write errors.  To avoid this, always explicitly
   *  re-select the chip after switching port directions.
   */
  CURRENT_CHIP = -1;
}


static void SetAddress(int addressValue) {
  outportb(IO_PORTA_ADDRESS, addressValue);
}


static void SetData(int dataValue) {
  outportb(IO_PORTB_ADDRESS, dataValue);
}


static int GetData(void) {
  return (inportb(IO_PORTB_ADDRESS));
}


static void SetControl(int controlPortValue) {
  /*
   * Possible optimization: Don't send value if already correct.
   */
  outportb(IO_PORTC_ADDRESS, controlPortValue);
}

static void selectBoard(int board) {
  SetAddress(board);
  SetControl(CTRL_BASE ^ CTRL_ADRSEL1);    /* put board ID onto address pins */
  ioDelay(max(DELAY_ADDRESS_SETTLE, DELAY_ADRSEL1_SETTLE));          /* wait */

  SetControl(CTRL_BASE ^ CTRL_ADRSEL1 ^ CTRL_ALE);          /* pull ALE high */
  ioDelay(DELAY_ALE_SETTLE);                                         /* wait */

  SetControl(CTRL_BASE ^ CTRL_ADRSEL1);                     /* pull ALE back */
  ioDelay(DELAY_ALE_SETTLE);                                         /* wait */

  SetControl(CTRL_BASE);                                     /* ADRSEL1 done */
  ioDelay(DELAY_ADRSEL1_SETTLE);

  CURRENT_BOARD = board;
  CURRENT_CHIP  = -1;
}


static void selectChip(int chip) {
  SetAddress(chip);                                           /* select chip */
  ioDelay(DELAY_ADDRESS_SETTLE);                                     /* wait */

  SetControl(CTRL_BASE ^ CTRL_ALE);                         /* pull ALE high */
  ioDelay(DELAY_ALE_SETTLE);                                         /* wait */

  SetControl(CTRL_BASE);                                    /* pull ALE back */
  ioDelay(DELAY_ALE_SETTLE);                                         /* wait */

  CURRENT_CHIP = chip;
}


void SetBaseAddress(int address) {
  IO_BASE_ADDRESS = address;
}


/*
 *  RESET A SINGLE BOARD
 *
 *  This function resets an entire board. It is not optimized for speed.
 *  It is necessary to delay after calling this function until the board
 *  reset completes.
 */
int ResetBoard(int board) {

  /* Configure the IO card (doesn't matter if for data input or output) */
  ConfigureIO_Port(CNFG_INPUT);                     /* configure the IO port */
  ConfigureIO_Port(CNFG_OUTPUT);                    /* configure the IO port */

  selectBoard(board);                                    /* select the board */

  SetControl(CTRL_BASE ^ CTRL_RST);                       /* RESET THE BOARD */
  ioDelay(DELAY_RST_HOLD);                                           /* wait */
  SetControl(CTRL_BASE);                                   /* stop resetting */
  ioDelay(DELAY_RST_RECOVER);                                        /* wait */

  CURRENT_BOARD = -1;                    /* reset this on next IO to be safe */
  CURRENT_CHIP  = -1;                               /* reset this to be safe */
  return (0);
}


void SetRegister(int board, int chip, int reg, int value) {
  if (CURRENT_PORT_CNFG != CNFG_OUTPUT)      /* set IO data lines for output */
    ConfigureIO_Port(CNFG_OUTPUT);
  if (CURRENT_BOARD != board)                 /* make sure board is selected */
    selectBoard(board);
  if (CURRENT_CHIP != chip)                    /* make sure chip is selected */
    selectChip(chip);

  SetAddress(reg);                               /* select the right address */
  SetData(value);                                         /* output the data */
  SetControl(CTRL_BASE ^ CTRL_ADRSEL2);                          /* pull low */
  ioDelay(max(max(DELAY_ADDRESS_SETTLE,DELAY_DATA_SETTLE),           /* wait */
          DELAY_ADRSEL2_SETTLE));
  SetControl(CTRL_BASE ^ CTRL_WRB ^ CTRL_ADRSEL2);           /* pull WRB low */
  ioDelay(DELAY_WRB_HOLD);                                        /* hold it */
  SetControl(CTRL_BASE ^ CTRL_ADRSEL2);                /* let WRB high again */
  ioDelay(DELAY_WRB_RECOVER);                                        /* wait */
  SetControl(CTRL_BASE);                               /* let WRB high again */
  ioDelay(DELAY_ADRSEL2_SETTLE);                                     /* wait */
}

int GetRegister(int board, int chip, int reg) {
  int rval;

  if (CURRENT_PORT_CNFG != CNFG_INPUT)        /* set IO data lines for input */
    ConfigureIO_Port(CNFG_INPUT);
  if (CURRENT_BOARD != board)                 /* make sure board is selected */
    selectBoard(board);
  if (CURRENT_CHIP != chip)                    /* make sure chip is selected */
    selectChip(chip);

  SetAddress(reg);                               /* select the right address */
  SetControl(CTRL_BASE ^ CTRL_ADRSEL2);                  /* pull adrsel2 low */
  ioDelay(max(DELAY_ADDRESS_SETTLE, DELAY_ADRSEL2_SETTLE));          /* wait */
  SetControl(CTRL_BASE ^ CTRL_RDB ^ CTRL_ADRSEL2);           /* pull RDB low */
  ioDelay(DELAY_RDB_HOLD);
  rval = GetData();
  SetControl(CTRL_BASE ^ CTRL_ADRSEL2);                      /* let RDB high */
  ioDelay(DELAY_RDB_RECOVER);
  SetControl(CTRL_BASE);                                 /* let ADRSEL2 high */
  ioDelay(DELAY_ADRSEL2_SETTLE);

  return (rval);
}

int CheckRegister(int board, int chip, int reg, int value) {
  int i;

  i = GetRegister(board, chip, reg);
  if (i != value)
    return (-1);
  return (0);
}

