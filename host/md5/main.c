#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#define N 10

unsigned char encrypt[] = "admin";
unsigned char decrypt[16];

int main(int argc, char *argv[])
{
	int i;

	MD5_CTX md5;

	MD5Init(&md5);
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, decrypt);

	printf("%s\n", encrypt);
//	printf("%s\n", decrypt);
	for (i = 0; i < 16; i++)
		printf("%02x", decrypt[i]);

	printf("\n");

	return 0;
}
