TARGETS = simple_ul_rx.out

SRC = test.cpp simple_ul_rx.cpp \
	  lte_phy.cpp parse_args.cpp \
	  OFDM.cpp res_mapper.cpp equalizer.cpp \
	  fft.cpp dmrs.cpp \
	  gauss.cpp general_func.cpp check.cpp 

SRC += ../timer/timer.c

OBJS := $(patsubst %.cpp,%.o, $(SRC))
OBJS := $(patsubst %.c,%.o, $(OBJS))

CXXFLAGS := -g -Wall
CFLAGS := -g -Wall

LINKFLAGS :=

ifeq ($(PG), 1)
	CXXFLAGS += -pg	
	CFLAGS += -pg	
	LINKFLAGS += -pg
endif


INC_FLAGS += -I. -I../timer
INC_FLAGS += -I../papi-rapl

CXXFLAGS += $(INC_FLAGS)
CFLAGS += $(INC_FLAGS)

LINKFLAGS += -lpthread
LINKFLAGS += -L../papi-rapl -lraplpower

#CXXFLAGS += -DPTH

simple_ul_rx.out: $(OBJS)
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(TARGETS)
