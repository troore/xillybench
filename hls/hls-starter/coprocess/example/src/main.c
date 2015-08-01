#include <math.h>
#include <stdint.h>
#include "xilly_debug.h"

extern float sinf(float);

int mycalc(int a, float *x2) {
  *x2 = sinf(*x2);
  return a + 1;
}

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

  uint32_t x1, tmp, y1;
  float x2, y2;

  xilly_puts("Hello, world\n");

  // Handle input data
  x1 = *in++;
  tmp = *in++;
  x2 = *((float *) &tmp); // Convert uint32_t to float

  // Debug output
  xilly_puts("x1=");
  xilly_decprint(x1, 1);
  xilly_puts("\n");

  // Run the calculations
  y1 = mycalc(x1, &x2);
  y2 = x2; // This helps HLS in the conversion below

  // Handle output data
  tmp = *((uint32_t *) &y2); // Convert float to uint32_t
  *out++ = y1;
  *out++ = tmp;
}
