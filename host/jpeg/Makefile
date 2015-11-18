TARGETS := jpeg2bmp.out

SRCS := main.c \
	jpeg2bmp.c \
	chenidct.c \
	decode.c \
	huffman.c \
	jfif_read.c \
	marker.c \
	../timer/timer.c

OBJS := $(patsubst %.c,%.o,$(SRCS))

INCDIRS := -I../timer -I../papi-rapl
CFLAGS := -g -Wall $(INCDIRS)
LINKFLAGS := -L../papi-rapl -lraplpower

jpeg2bmp.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -rf *.o $(TARGETS)
