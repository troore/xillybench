#ifndef __MD5_H_
#define __MD5_H_

#define N 64 // text length
#define NFS 100000 // number of frames in a superframe

typedef unsigned int uint32;

struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf,
	       unsigned len);
void MD5Final(struct MD5Context *context, unsigned char digest[16]);
void MD5Transform(uint32 buf[4], uint32 const in[16]);

void MD5Iterate(unsigned char ctx_in[64], uint32 ctx_buf[4], unsigned char const buf[N]);

void byteReverse(unsigned char *buf, uint32 longs);
void byteReverse_bounded(unsigned char buf[64]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

#endif /* !__MD5_H_ */
