
TARGETS := a.out

CFLAGS := -g -Wall

CFLAGS += -I./ -I../../timer

SRCS := main.c dct_prof.c ../../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

a.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
