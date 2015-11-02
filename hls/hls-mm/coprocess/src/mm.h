#ifndef __MM_H_
#define __MM_H_

#define N	(1 << 8)

//#define OPT

// Prototype of top level function for C-synthesis
void mm_base (
      int a[(N / 2)][N],
      int b[N][N],
      int c[(N / 2)][N]);

void mm_opt (
      int a[(N / 2)][N],
      int b[N][N],
      int c[(N / 2)][N]);

#endif // __MM_H_ not defined
