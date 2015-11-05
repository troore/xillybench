#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dct.h"

void dct(dct_data_t *input, dct_data_t *output)
{
	int fdr, fdw;
	int i;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	for (i = 0; i < FF; i++) {
		write(fdw, (void *)input, N * sizeof(dct_data_t));
		read(fdr, (void *)output, N * sizeof(dct_data_t));
	}

	close(fdr);
	close(fdw);
}
