1. Weird constant loopback latency.

	int a[N], b[N];

	for (i = 0; i < N; i++)
		a[i] = *in++;

	for (i = 0; i < N; i++) {
	//	b[i] = 3;
		b[i] = a[i];
		*out++ = b[i];
	}

If we use "b[i]=3" as output, the loopback time is 0.05ms. If we use "b[i]=a[i]" instead, it is 10ms. Why?
