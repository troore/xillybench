TARGETS := md5.out

SRCS := main.c md5.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

CFLAGS := -g -Wall
LINKFLAGS :=

md5.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -rf *.o $(TARGETS)
