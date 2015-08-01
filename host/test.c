#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *p;
	int a;
	float b, c;

	p = (void *)malloc(2 * sizeof(int));

	p[0] = 2;
	((float *)p)[1] = 3.1415;
	a = p[1];
	b = *((float *)(&p[1]));
	c = ((float *)p)[1];


	printf("a = %d, b = %f, c = %f\n", a, b, c);

	return 0;
}
