
Reasons why multithreading version is slower than single threaded version:

1. workload of a single thread is small.

2. overhead of launching a thread (by reduce the times of launching).

3. pipeline latency overhead (sync_threads(), by merge).

4. block_memcpy within pipeline stages.
