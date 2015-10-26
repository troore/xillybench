#include "dct.h"

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return
   
   short a[N], b[N];
   int i;

   for(i = 0; i < N; i++) {
      a[i] = (short) *in++;  // convert unit32_t to short 
   } 

   dct_opt(a, b);  // execute dct_opt function

   for(i = 0; i < N; i++) {
      *out++ = (uint32_t) b[i];  // convert short to int
   }
}
