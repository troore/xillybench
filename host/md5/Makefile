TARGETS := md5.out

SRCS := main.c md5.c ../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

INCDIRS := -I../timer
CFLAGS := -g -Wall $(INCDIRS)
LINKFLAGS :=

md5.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -rf *.o $(TARGETS)
