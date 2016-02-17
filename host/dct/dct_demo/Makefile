
TARGETS := dct_host.out dct_co.out

SRCS_HOST := main.c dct_host.c ../../timer/timer.c
OBJS_HOST := $(patsubst %.c,%.o,$(SRCS_HOST))
SRCS_CO := $(subst host,co,$(SRCS_HOST)) 
OBJS_CO := $(patsubst %.c,%.o,$(SRCS_CO))
	
#$(warning $(SRCS_CO))

CFLAGS := -g -Wall
LINKFLAGS := -L../../papi-rapl -lraplpower

CFLAGS += -I./ -I../../timer -I../../papi-rapl


dct_host.out: $(OBJS_HOST)
	$(CC) -o $@ $^ $(LINKFLAGS)

dct_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
