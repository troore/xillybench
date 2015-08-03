

#ifndef __MEAS_H_
#define __MEAS_H_

#include <sys/time.h>

#define TIME_MEASURE_WRAPPER_VECTOR(BODY, i)				\
{	\
	start = clock(); \
	BODY; \
	end = clock(); \
	cost[i] += (end - start);	\
}

#define TIME_MEASURE_WRAPPER_SCALAR(BODY)				\
{	\
	double start, end;	\
	start = clock(); \
	BODY; \
	end = clock(); \
	printf("%lfms\n", ((end - start) * 1000.0 / CLOCKS_PER_SEC)); \
}

double dtime();

#endif
