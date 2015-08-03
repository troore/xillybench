#include <stdint.h>
#include "dct_base.h"
#include "dct_opt.h"

void xillybus_wrapper(int *in, int *out) {
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return
   
   short a[N], b[N];
   int i, sel;

   sel = *in++; // get the select info 0: dct_base  1: dct_opt

   for(i = 0; i < N; i++) {
      a[i] = (short) *in++;  // convert unit32_t to short 
   } 

   if(sel == 0) {
      dct_base(a, b);   // execute dct_base function
   }
   else if (sel == 1) {
      dct_opt(a, b);  // execute dct_opt function
   }
   else;

   for(i = 0; i < N; i++) {
      *out++ = (uint32_t) b[i];  // convert short to int
   }
}
