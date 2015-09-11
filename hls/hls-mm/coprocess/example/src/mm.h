#ifndef __MM_H__
#define __MM_H__

#define N	(1 << 3)

// Prototype of top level function for C-synthesis
void mm_base (
      int a[N][N],
      int b[N][N],
      int c[N][N]);

void mm_opt (
      int a[N][N],
      int b[N][N],
      int c[N][N]);

#endif // __MM_H__ not defined
