-include .config.mk

all: spike-vp

CXX ?= g++

CXXFLAGS ?= -g -std=c++17 

INCLUDE ?= -Iinclude -Iinstall-systemc-2.3.3/include -Iinstall-vcml/include -Iinstall-riscv-isa-sim/include

SRC_FILES = ./src/main.cpp ./src/spikevp/system.cpp ./src/spikevp/cpu.cpp

ARCHIVES = install-vcml/lib/libvcmld.a install-vcml/lib/libmwrd.a install-systemc-2.3.3/lib-linux64/libsystemc.a install-riscv-isa-sim/lib/libriscv.a install-riscv-isa-sim/lib/libsoftfloat.a install-riscv-isa-sim/lib/libdisasm.a 

LIBS ?= -lpthread -lelf -lstdc++fs -ldl -lrt 

spike-vp: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SRC_FILES) $(ARCHIVES) $(LIBS) -o ./spike-vp

clean:
	rm -f ./spike-vp
