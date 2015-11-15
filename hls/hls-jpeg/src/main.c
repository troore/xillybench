#include <stdio.h>
#include <stdlib.h>

#include "decode.h"
#include "jpeg2bmp.h"

int main_result;

	int
main ()
{
	main_result = 0;

	jpeg2bmp_main();

	printf ("%d\n", main_result);

//	return main_result;
	return 0;
}
