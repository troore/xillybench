#ifndef __MM_H_
#define __MM_H_

#define N	(1 << 8)

// Prototype of top level function for C-synthesis
/*
void mm_base (
      int a[N][N],
      int b[N][N],
      int c[N][N]);
	  */
void mm_base(int *a, int *b, int *c);

/*
void mm_opt (
      int a[N][N],
      int b[N][N],
      int c[N][N]);
	  */
void mm_opt(int *a, int *b, int *c);

#endif // __MM_H__ not defined
