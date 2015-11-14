#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "decode.h"
#include "init.h"
#include "huffman.h"

int
main ()
{
  main_result = 0;
  jpeg2bmp_main ();

  printf ("%d\n", main_result);

  return main_result;
}
