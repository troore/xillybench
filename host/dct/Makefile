TARGETS := dct.out dct_base.out dct_pth.out dct_pth_mutex.out dct_pth_co.out

OBJS := \
	dct.o\
	dct_test.o\
	../timer/timer.o\

OBJS_BASE := \
	dct_base.o\
	dct_test.o\
	../timer/timer.o\

OBJS_PTH_CO := \
	dct_pth_co.o\
	dct_test_co.o\
	../timer/timer.o\

OBJS_PTH := $(subst base,pth,$(OBJS_BASE))
OBJS_PTH_MUTEX := $(subst base,pth_mutex,$(OBJS_BASE))
#OBJS_PTH_CO := $(subst base,pth_co,$(OBJS_BASE))

LDFLAGS_BASE :=
LDFLAGS_PTH := $(LDFLAGS_BASE) -lpthread

CFLAGS := -g -Wall
CFLAGS += -I../timer
#CFLAGS += -DFPGA

dct.out: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS_BASE)

dct_base.out: $(OBJS_BASE)
	$(CC) -o $@ $^ $(LDFLAGS_BASE)

dct_pth.out: $(OBJS_PTH)
	$(CC) -o $@ $^ $(LDFLAGS_PTH)

dct_pth_mutex.out: $(OBJS_PTH_MUTEX)
	$(CC) -o $@ $^ $(LDFLAGS_PTH)

dct_pth_co.out: $(OBJS_PTH_CO)
	$(CC) -o $@ $^ $(LDFLAGS_PTH)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGETS) $(OBJS) $(OBJS_BASE) $(OBJS_PTH) $(OBJS_PTH_MUTEX) $(OBJS_PTH_CO)
