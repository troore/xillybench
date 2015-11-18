
TARGETS := a.out

SRCS := main.c dct.c ../../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

CFLAGS := -g -Wall
LINKFLAGS := -L../../papi-rapl -lraplpower

CFLAGS += -I./ -I../../timer -I../../papi-rapl


a.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
