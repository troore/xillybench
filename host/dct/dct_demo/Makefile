
TARGETS := dct_host.out dct_co.out

SRCS_HOST := main.c dct_wrapper.c dct_2d_host.c transpose_host.c
SRCS_HOST += ../../timer/timer.c
OBJS_HOST := $(patsubst %.c,%.o,$(SRCS_HOST))

SRCS_CO := main.c dct_wrapper.c dct_2d_co.c transpose_host.c
SRCS_CO += ../../timer/timer.c
OBJS_CO := $(patsubst %.c,%.o,$(SRCS_CO))
	
#$(warning $(SRCS_CO))

CFLAGS := -g -Wall
LINKFLAGS := -L../../papi-rapl -lraplpower

CFLAGS += -I./ -I../../timer -I../../papi-rapl
CFLAGS += -DCO

dct_host.out: $(OBJS_HOST)
	$(CC) -o $@ $^ $(LINKFLAGS)

dct_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

distclean:
	rm -rf $(TARGETS)
