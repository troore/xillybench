TARGETS := md5.out

SRCS := main.c md5.c ../timer/timer.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

INCDIRS := -I../timer -I../papi-rapl
CFLAGS := -g -Wall $(INCDIRS)
LINKFLAGS := -L../papi-rapl -lraplpower

md5.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -rf *.o $(TARGETS)
