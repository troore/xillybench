#include <stdio.h>
#include <unistd.h>
#include "rapl_power.h"

int main(int argc, char *argv[])
{
	rapl_power_start();
	sleep(10);
	rapl_power_stop();

	return 0;
}
