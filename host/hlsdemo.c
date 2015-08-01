#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

  int fdr, fdw;

  struct {
    uint32_t v1;
    float v2;
  } tologic, fromlogic;

  fdr = open("/dev/xillybus_read_32", O_RDONLY);
  fdw = open("/dev/xillybus_write_32", O_WRONLY);

  if ((fdr < 0) || (fdw < 0)) {
    perror("Failed to open Xillybus device file(s)");
    exit(1);
  }

  tologic.v1 = 123;
  tologic.v2 = 0.78539816; // ~ pi/4

  // Not checking return values of write() and read(). This must be done
  // in a real-life program to ensure reliability.

  write(fdw, (void *) &tologic, sizeof(tologic));
  read(fdr, (void *) &fromlogic, sizeof(fromlogic));

  printf("FPGA said: %d + 1 = %d and also "
	 "sin(%f) = %f\n",
	 tologic.v1, fromlogic.v1,
	 tologic.v2, fromlogic.v2);

  close(fdr);
  close(fdw);

  return 0;
}