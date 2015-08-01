#include <stdio.h>
#include <unistd.h>

int main () {
  int rc;

  rc = execl("/usr/local/bin/xillydiag.pl", "xilly-diagnostic", NULL);

  if (rc)
    perror("Failed to execute");

  return rc;
}
