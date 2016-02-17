TARGETS = test.out test_co.out

SRC = test.cpp simple_ul_rx.cpp \
	  lte_phy.cpp parse_args.cpp \
	  OFDM.cpp res_mapper.cpp equalizer.cpp \
	  fft.cpp dmrs.cpp \
	  gauss.cpp general_func.cpp check.cpp 

SRC += ../../timer/timer.c

SRC_CO = $(subst OFDM,OFDM_co,$(SRC))

#$(warning $(SRC_CO))

OBJS := $(patsubst %.cpp,%.o, $(SRC))
OBJS := $(patsubst %.c,%.o, $(OBJS))

OBJS_CO = $(subst OFDM,OFDM_co,$(OBJS))

#$(warning $(OBJS_CO))

CXXFLAGS := -g -Wall
CFLAGS := -g -Wall

LINKFLAGS :=

INC_FLAGS += -I. -I../../timer
INC_FLAGS += -I../../papi-rapl

CXXFLAGS += $(INC_FLAGS)
CFLAGS += $(INC_FLAGS)

LINKFLAGS += -lpthread
LINKFLAGS += -L../../papi-rapl -lraplpower

test.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

test_co.out: $(OBJS_CO)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
