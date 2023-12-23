-include .config.mk

all: spike-vp

CXX ?= g++

CXXFLAGS ?= -g -std=c++17 

INCLUDE ?= -Iinclude -I$(SYSTEMC_HOME)/include -I$(VCML_HOME)/include -I$(SPIKE_HOME)/include

SRC_FILES = ./src/main.cpp ./src/spikevp/system.cpp ./src/spikevp/cpu.cpp

ARCHIVES = $(VCML_HOME)/lib/libvcmld.a $(VCML_HOME)/lib/libmwrd.a $(SYSTEMC_HOME)/lib-linux64/libsystemc.a $(SPIKE_HOME)/lib/libriscv.a $(SPIKE_HOME)/lib/libsoftfloat.a $(SPIKE_HOME)/lib/libdisasm.a 

LIBS ?= -lpthread -lelf -lstdc++fs -ldl -lrt 

spike-vp: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SRC_FILES) $(ARCHIVES) $(LIBS) -o ./spike-vp

clean:
	rm -f ./spike-vp
