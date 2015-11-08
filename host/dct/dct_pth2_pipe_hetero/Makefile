TARGETS := dct_cpu.out dct_cpu_omp.out dct_fpga.out dct_co.out dct_pipe_host.out dct_pipe_co.out

OBJS_CPU := \
	dct_test.o\
	dct_cpu.o\
	../../timer/timer.o\

OBJS_CPU_OMP := $(subst cpu,cpu_omp, $(OBJS_CPU))
OBJS_FPGA := $(subst cpu,fpga, $(OBJS_CPU))
OBJS_CO := $(subst cpu,co, $(OBJS_CPU))
OBJS_PIPE_HOST := $(subst cpu,pipe_host, $(OBJS_CPU))
OBJS_PIPE_CO := $(subst cpu,pipe_co, $(OBJS_CPU))

CFLAGS := -g -Wall
CFLAGS += -I../../timer -I../../papi-rapl
#CFLAGS += -DFPGA

LDFLAGS := -L../../papi-rapl -lraplpower
LDFLAGS_PTH := $(LDFLAGS) -lpthread

dct_cpu.out: $(OBJS_CPU)
	$(CC) -o $@ $^ $(LDFLAGS)

dct_cpu_omp.out: $(OBJS_CPU_OMP)
	$(CC) -o $@ $^ $(LDFLAGS) -fopenmp

dct_fpga.out: $(OBJS_FPGA)
	$(CC) -o $@ $^ $(LDFLAGS)

dct_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LDFLAGS)

dct_pipe_host.out: $(OBJS_PIPE_HOST)
	$(CC) -o $@ $^ $(LDFLAGS_PTH)

dct_pipe_co.out: $(OBJS_PIPE_CO)
	$(CC) -o $@ $^ $(LDFLAGS_PTH)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

dct_cpu_omp.o: dct_cpu_omp.c
	$(CC) -o $@ -c $< $(CFLAGS) -fopenmp

clean:
	rm -rf *.o $(TARGETS)
