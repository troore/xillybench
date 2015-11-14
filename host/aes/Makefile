TARGETS := aes.out

SRCS := main.c aes.c ../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

INCDIRS := -I../timer
CFLAGS := -g -Wall $(INCDIRS)
LINKFLAGS :=

aes.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -rf *.o $(TARGETS)
