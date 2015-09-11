TARGET := mm.out

OBJS := mm.o ../timer/timer.o

CFLAGS := -g -Wall -I../timer

CFLAGS += -DXILLY
#CFLAGS += -DGOLDEN

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJS)
