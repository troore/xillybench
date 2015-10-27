#include "dct_2d.h"
#include "xilly_debug.h"

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return
   
   int a[N], b[N];
   int i;
   int flag;

   xilly_puts("Hello, world!\n");

   flag = *in++;
   for (i = 0; i < N; i++) {
      a[i] = (int)*in++;  // convert unit32_t to short 
   } 

   if (flag == 0)
	   half_dct_2d_0(a, b);
   else
	   half_dct_2d_1(a, b);

   for (i = 0; i < N; i++) {
      *out++ = (int)b[i];  // convert short to int
   }
}
