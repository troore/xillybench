#include <string.h>		/* for memcpy() */

#include "md5.h"

/*
 * Note: this code is harmless on little-endian machines.
 */
void byteReverse(unsigned char *buf, uint32 longs)
{
	int i;
	uint32 t;

	for (i = 0; i < longs; i++) {
		t = (uint32) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
			((unsigned) buf[1] << 8 | buf[0]);
		*(uint32 *)buf = t;
		buf += 4;
	}
}

void byteReverse_bounded(unsigned char buf[64])
{
	int i;
	uint32 t;

	int h;

	for (h = 0; h < NFS; h++) {
	for (i = 0; i < 16; i++) {
		t = (uint32) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
			((unsigned) buf[1] << 8 | buf[0]);
		*(uint32 *)buf = t;
		buf += 4;
	}
	
	buf -= (4 * 16);
	}
}


/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5Init(struct MD5Context *ctx)
{
	ctx->buf[0] = 0x67452301;
	ctx->buf[1] = 0xefcdab89;
	ctx->buf[2] = 0x98badcfe;
	ctx->buf[3] = 0x10325476;

	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len)
{
	uint32 t;

	/* Update bitcount */

	t = ctx->bits[0];
	if ((ctx->bits[0] = t + ((uint32) len << 3)) < t)
		ctx->bits[1]++;		/* Carry from low to high */
	ctx->bits[1] += len >> 29;

	t = (t >> 3) & 0x3f;	/* Bytes already in shsInfo->data */

	/* Handle any leading odd-sized chunks */

	if (t) {
		unsigned char *p = (unsigned char *)ctx->in + t;

		t = 64 - t;
		if (len < t) {
			memcpy(p, buf, len);
			return;
		}
		memcpy(p, buf, t);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *)ctx->in);
		buf += t;
		len -= t;
	}

	/* Process data in 64-byte chunks */

	MD5Iterate(ctx->in, ctx->buf, buf);
	/*
	while (len >= 64) {
		memcpy(ctx->in, buf, 64);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *)ctx->in);
		buf += 64;
		len -= 64;
	}
	*/

	/* Handle any remaining bytes of data. */

//	memcpy(ctx->in, buf, len);
}

void MD5Iterate(unsigned char ctx_in[64], uint32 ctx_buf[4], unsigned char const buf[N])
{
	int i, j;
//	uint32 n_iters = len / 64;
	uint32 n_iters = 10;

	for (i = 0; i < n_iters; i++) {
		for (j = 0; j < 64; j++)
			ctx_in[j] = buf[i * 64 + j];

		byteReverse_bounded(ctx_in);
		MD5Transform(ctx_buf, (uint32 *)ctx_in);
	}
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void MD5Final(struct MD5Context *ctx, unsigned char digest[16])
{
	unsigned count;
	unsigned char *p;

	/* Compute number of bytes mod 64 */
	count = (ctx->bits[0] >> 3) & 0x3F;

	/* Set the first char of padding to 0x80.  This is safe since there is
	   always at least one byte free */
	p = ctx->in + count;
	*p++ = 0x80;

	/* Bytes of padding needed to make 64 bytes */
	count = 64 - 1 - count;

	/* Pad out to 56 mod 64 */
	if (count < 8) {
		/* Two lots of padding:  Pad the first block to 64 bytes */
		memset(p, 0, count);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *) ctx->in);

		/* Now fill the next block with 56 bytes */
		memset(ctx->in, 0, 56);
	} else {
		/* Pad block to 56 bytes */
		memset(p, 0, count - 8);
	}
	byteReverse(ctx->in, 14);

	/* Append length in bits and transform */
	((uint32 *) ctx->in)[14] = ctx->bits[0];
	((uint32 *) ctx->in)[15] = ctx->bits[1];

	MD5Transform(ctx->buf, (uint32 *) ctx->in);
	byteReverse((unsigned char *) ctx->buf, 4);
	memcpy(digest, ctx->buf, 16);
	//    memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
	ctx = 0;
}

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f /*(x, y, z)*/ + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void MD5Transform(uint32 buf[4], uint32 const in[16])
{
	register uint32 a, b, c, d;

	int h;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	for (h = 0; h < NFS; h++) {
	MD5STEP(F1(b,c,d), a, b, c, d, in[0] + 0xd76aa478L, 7);
	MD5STEP(F1(a,b,c), d, a, b, c, in[1] + 0xe8c7b756L, 12);
	MD5STEP(F1(d,a,b), c, d, a, b, in[2] + 0x242070dbL, 17);
	MD5STEP(F1(c,d,a), b, c, d, a, in[3] + 0xc1bdceeeL, 22);
	MD5STEP(F1(b,c,d), a, b, c, d, in[4] + 0xf57c0fafL, 7);
	MD5STEP(F1(a,b,c), d, a, b, c, in[5] + 0x4787c62aL, 12);
	MD5STEP(F1(d,a,b), c, d, a, b, in[6] + 0xa8304613L, 17);
	MD5STEP(F1(c,d,a), b, c, d, a, in[7] + 0xfd469501L, 22);
	MD5STEP(F1(b,c,d), a, b, c, d, in[8] + 0x698098d8L, 7);
	MD5STEP(F1(a,b,c), d, a, b, c, in[9] + 0x8b44f7afL, 12);
	MD5STEP(F1(d,a,b), c, d, a, b, in[10] + 0xffff5bb1L, 17);
	MD5STEP(F1(c,d,a), b, c, d, a, in[11] + 0x895cd7beL, 22);
	MD5STEP(F1(b,c,d), a, b, c, d, in[12] + 0x6b901122L, 7);
	MD5STEP(F1(a,b,c), d, a, b, c, in[13] + 0xfd987193L, 12);
	MD5STEP(F1(d,a,b), c, d, a, b, in[14] + 0xa679438eL, 17);
	MD5STEP(F1(c,d,a), b, c, d, a, in[15] + 0x49b40821L, 22);

	MD5STEP(F2(b,c,d), a, b, c, d, in[1] + 0xf61e2562L, 5);
	MD5STEP(F2(a,b,c), d, a, b, c, in[6] + 0xc040b340L, 9);
	MD5STEP(F2(d,a,b), c, d, a, b, in[11] + 0x265e5a51L, 14);
	MD5STEP(F2(c,d,a), b, c, d, a, in[0] + 0xe9b6c7aaL, 20);
	MD5STEP(F2(b,c,d), a, b, c, d, in[5] + 0xd62f105dL, 5);
	MD5STEP(F2(a,b,c), d, a, b, c, in[10] + 0x02441453L, 9);
	MD5STEP(F2(d,a,b), c, d, a, b, in[15] + 0xd8a1e681L, 14);
	MD5STEP(F2(c,d,a), b, c, d, a, in[4] + 0xe7d3fbc8L, 20);
	MD5STEP(F2(b,c,d), a, b, c, d, in[9] + 0x21e1cde6L, 5);
	MD5STEP(F2(a,b,c), d, a, b, c, in[14] + 0xc33707d6L, 9);
	MD5STEP(F2(d,a,b), c, d, a, b, in[3] + 0xf4d50d87L, 14);
	MD5STEP(F2(c,d,a), b, c, d, a, in[8] + 0x455a14edL, 20);
	MD5STEP(F2(b,c,d), a, b, c, d, in[13] + 0xa9e3e905L, 5);
	MD5STEP(F2(a,b,c), d, a, b, c, in[2] + 0xfcefa3f8L, 9);
	MD5STEP(F2(d,a,b), c, d, a, b, in[7] + 0x676f02d9L, 14);
	MD5STEP(F2(c,d,a), b, c, d, a, in[12] + 0x8d2a4c8aL, 20);

	MD5STEP(F3(b,c,d), a, b, c, d, in[5] + 0xfffa3942L, 4);
	MD5STEP(F3(a,b,c), d, a, b, c, in[8] + 0x8771f681L, 11);
	MD5STEP(F3(d,a,b), c, d, a, b, in[11] + 0x6d9d6122L, 16);
	MD5STEP(F3(c,d,a), b, c, d, a, in[14] + 0xfde5380cL, 23);
	MD5STEP(F3(b,c,d), a, b, c, d, in[1] + 0xa4beea44L, 4);
	MD5STEP(F3(a,b,c), d, a, b, c, in[4] + 0x4bdecfa9L, 11);
	MD5STEP(F3(d,a,b), c, d, a, b, in[7] + 0xf6bb4b60L, 16);
	MD5STEP(F3(c,d,a), b, c, d, a, in[10] + 0xbebfbc70L, 23);
	MD5STEP(F3(b,c,d), a, b, c, d, in[13] + 0x289b7ec6L, 4);
	MD5STEP(F3(a,b,c), d, a, b, c, in[0] + 0xeaa127faL, 11);
	MD5STEP(F3(d,a,b), c, d, a, b, in[3] + 0xd4ef3085L, 16);
	MD5STEP(F3(c,d,a), b, c, d, a, in[6] + 0x04881d05L, 23);
	MD5STEP(F3(b,c,d), a, b, c, d, in[9] + 0xd9d4d039L, 4);
	MD5STEP(F3(a,b,c), d, a, b, c, in[12] + 0xe6db99e5L, 11);
	MD5STEP(F3(d,a,b), c, d, a, b, in[15] + 0x1fa27cf8L, 16);
	MD5STEP(F3(c,d,a), b, c, d, a, in[2] + 0xc4ac5665L, 23);

	MD5STEP(F4(b,c,d), a, b, c, d, in[0] + 0xf4292244L, 6);
	MD5STEP(F4(a,b,c), d, a, b, c, in[7] + 0x432aff97L, 10);
	MD5STEP(F4(d,a,b), c, d, a, b, in[14] + 0xab9423a7L, 15);
	MD5STEP(F4(c,d,a), b, c, d, a, in[5] + 0xfc93a039L, 21);
	MD5STEP(F4(b,c,d), a, b, c, d, in[12] + 0x655b59c3L, 6);
	MD5STEP(F4(a,b,c), d, a, b, c, in[3] + 0x8f0ccc92L, 10);
	MD5STEP(F4(d,a,b), c, d, a, b, in[10] + 0xffeff47dL, 15);
	MD5STEP(F4(c,d,a), b, c, d, a, in[1] + 0x85845dd1L, 21);
	MD5STEP(F4(b,c,d), a, b, c, d, in[8] + 0x6fa87e4fL, 6);
	MD5STEP(F4(a,b,c), d, a, b, c, in[15] + 0xfe2ce6e0L, 10);
	MD5STEP(F4(d,a,b), c, d, a, b, in[6] + 0xa3014314L, 15);
	MD5STEP(F4(c,d,a), b, c, d, a, in[13] + 0x4e0811a1L, 21);
	MD5STEP(F4(b,c,d), a, b, c, d, in[4] + 0xf7537e82L, 6);
	MD5STEP(F4(a,b,c), d, a, b, c, in[11] + 0xbd3af235L, 10);
	MD5STEP(F4(d,a,b), c, d, a, b, in[2] + 0x2ad7d2bbL, 15);
	MD5STEP(F4(c,d,a), b, c, d, a, in[9] + 0xeb86d391L, 21);
	}

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}
