TARGETS := dct.out dct_base.out dct_pth.out dct_pth_mutex.out

OBJS := \
	dct.o\
	dct_test.o\

OBJS_BASE := \
	dct_base.o\
	dct_test.o\

OBJS_PTH := $(subst base,pth,$(OBJS_BASE))
OBJS_PTH_MUTEX := $(subst base,pth_mutex,$(OBJS_BASE))

LDFLAGS_BASE :=
LDFLAGS_PTH := $(LDFLAGS_BASE) -lpthread

CXXFLAGS := -g -Wall

dct.out: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS_BASE)

dct_base.out: $(OBJS_BASE)
	$(CXX) -o $@ $^ $(LDFLAGS_BASE)

dct_pth.out: $(OBJS_PTH)
	$(CXX) -o $@ $^ $(LDFLAGS_PTH)

dct_pth_mutex.out: $(OBJS_PTH_MUTEX)
	$(CXX) -o $@ $^ $(LDFLAGS_PTH)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(TARGETS) $(OBJS) $(OBJS_BASE) $(OBJS_PTH) $(OBJS_PTH_MUTEX)
