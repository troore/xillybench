
TARGETS := dct.out dct_co.out

CFLAGS := -g -Wall

CFLAGS += -I./ -I../ -I../../timer -I../../papi-rapl

SRCS := dct_2d_test.c dct_2d_host.c ../../timer/timer.c
SRCS_CO := dct_2d_co_test.c ../../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))
OBJS_CO := $(patsubst %.c,%.o,$(SRCS_CO))

LINKFLAGS := -L../../papi-rapl -lraplpower

dct.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

dct_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
