TARGETS := mm.out mm_co.out mm_wr.out mm_pth.out

OBJS := mm.o ../timer/timer.o
OBJS_CO = $(subst mm,mm_co,$(OBJS))
OBJS_WR = $(subst mm,mm_wr,$(OBJS))
OBJS_PTH = $(subst mm,mm_pth,$(OBJS))

CFLAGS := -g -Wall -I../timer

#CFLAGS += -DXILLY
#CFLAGS += -DGOLDEN

mm.out: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

mm_wr.out: $(OBJS_WR)
	$(CC) -o $@ $^ $(LDFLAGS)

mm_pth.out: $(OBJS_PTH)
	$(CC) -o $@ $^ $(LDFLAGS) -lpthread

mm_co.out: $(OBJS_CO)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGETS) $(OBJS) $(OBJS_CO) $(OBJS_WR)
