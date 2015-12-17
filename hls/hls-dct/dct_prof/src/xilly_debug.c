#include "xilly_debug.h"

volatile char debug_out;
volatile char debug_ready;

void xilly_puts(const char *str) {
  while (*str) {
    while (!(debug_ready & 1)); // Wait for ready signal
    debug_out = *str++;
  }
}

void xilly_hexprint(const uint32_t val, const int digits) {
  char out[9];
  int i;
  int first = 8 - digits;
  uint32_t v = val;

  for (i=7; i>=0; i--, v=v>>4) {
    char x = v & 0xf;
    out[i] = (x < 10) ? 48 + x : 87 + x;
    if ((x != 0) && (first > i))
      first = i;
  }
  out[8] = 0;

  xilly_puts(out + first);
}

void xilly_decprint(const uint32_t val, const int digits) {
  const uint32_t powers10[] = {1000000000, 100000000, 10000000, 1000000, 
			       100000, 10000, 1000, 100, 10, 1};
  char out[11];
  int i;
  int first = 10 - digits;
  uint32_t v = val;

  for (i=0; i<10; i++) {
    char x = 0;
    while (v >= powers10[i]) {
      v -= powers10[i];
      x++;
    }

    out[i] = 0x30 | x;

    if ((x != 0) && (first > i))
      first = i;
  }

  out[10] = 0;

  xilly_puts(out + first);
}
