**[2015-10-29]**

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


**[2015-10-31]**

mm cannot be synthesized by vivado when "N" is larger (e.g., 2^7), and the optimized version is more harsher.

**[2015-11-02]**

1. Solve problem of incorrect read results. You have to use a "while" loop to read many times until all results are collected, like the following:

	while (donebytes < n) {
		rc = read(fdr, (void *)out + donebytes, n - donebytes);
		donebytes += rc;
	}

2. The "read" in the pthread version mm/mm_pth.c cannot respond. The motivation of this example is to separate "write" and "read" in two threads.
