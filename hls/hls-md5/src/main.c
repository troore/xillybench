#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

//unsigned char encrypt[] = "admin";
unsigned char encrypt[N + 1];
unsigned char decrypt[16];

int main(int argc, char *argv[])
{
	int i;

	MD5_CTX md5;

	// privide _encrypt_
	for (i = 0; i < N; i++)
		encrypt[i] = 'a';
	encrypt[N] = '\0';

	MD5Init(&md5);
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, decrypt);

	printf("%d\n", strlen(encrypt));

	printf("%s\n", encrypt);
//	printf("%s\n", decrypt);
	for (i = 0; i < 16; i++)
		printf("%02x", decrypt[i]);

	printf("\n");

	return 0;
}
