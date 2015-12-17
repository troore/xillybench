
TARGETS := sleep.out

CFLAGS := -g -Wall

CFLAGS += -I./ -I../papi-rapl

SRCS := sleep.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

LINKFLAGS := -L../papi-rapl -lraplpower

sleep.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
