#include <stdio.h>

#include "aes.h"

/* ***************** main **************************** */
int
main ()
{
      int main_result = 0;

      main_result = aes_main();
      printf ("%d\n", main_result);

      return main_result;
}
