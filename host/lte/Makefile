include ../../common/makefile.config

TARGETS = SimpleULRx.out

SRC = SimpleULRx.cpp test.cpp
SRC += $(SRC_DIR)/lte_phy.cpp $(SRC_DIR)/parse_args.cpp
SRC += $(SRC_DIR)/OFDM/OFDM.cpp $(SRC_DIR)/ResMapping/ResMapper.cpp $(SRC_DIR)/Equalizing/Equalizer.cpp
SRC += $(LIB_DIR)/gauss.cpp $(LIB_DIR)/GeneralFunc.cpp $(LIB_DIR)/refs/dmrs.cpp $(LIB_DIR)/timer/timer.cpp $(LIB_DIR)/check/check.cpp $(LIB_DIR)/timer/timer.cpp

OBJS := $(patsubst %.cpp,%.o, $(SRC))

INC_FLAGS += -I. -I.. -I$(SRC_DIR) -I$(LIB_DIR)
INC_FLAGS += -I$(SRC_DIR)/OFDM -I$(SRC_DIR)/ResMapping -I$(SRC_DIR)/Equalizing

LINKFLAGS += -L$(LIB_DIR)/fft -lfft -lpthread

#CXXFLAGS += -DPTH


SimpleULRx.out: $(OBJS)
	$(MAKE) -C $(LIB_DIR)/fft libfft.a
	$(CXX) $^ $(LINKFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf *.o $(TARGETS)

