TARGETS := mm_host.out mm_co.out mm_wr.out mm_pth.out

OBJS := main.o mm_host.o ../timer/timer.o
OBJS_CO = $(subst host,co,$(OBJS))
OBJS_WR = $(subst host,wr,$(OBJS))
OBJS_PTH = $(subst host,pth,$(OBJS))

CFLAGS := -g -Wall -I../timer -I../papi-rapl
LINKFLAGS += -L../papi-rapl -lraplpower

#CFLAGS += -DXILLY
#CFLAGS += -DGOLDEN

mm_host.out: $(OBJS)
	$(CC) -o $@ $^ $(LINKFLAGS)

mm_wr.out: $(OBJS_WR)
	$(CC) -o $@ $^ $(LINKFLAGS)

mm_pth.out: $(OBJS_PTH)
	$(CC) -o $@ $^ $(LINKFLAGS) -lpthread

mm_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LINKFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
