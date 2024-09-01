VERILATOR = verilator
VERILATOR_ROOT ?= $(shell $(VERILATOR) -V | grep VERILATOR_ROOT | head -1 | awk '{print $$3}')
VINC = $(VERILATOR_ROOT)/include

CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(VINC) -I../generated
LDFLAGS = -L$(VERILATOR_ROOT)/lib -lverilated -lverilated_threads -lstdc++

all: sim

sim: VMatrixMultiplier.mk
	make -j -C obj_dir -f VMatrixMultiplier.mk
	make -j -C obj_dir -f VMatrixMultiplier.mk matrix_multiplier_sim

VMatrixMultiplier.mk: ../generated/MatrixMultiplier.v matrix_multiplier_sim.cpp
	$(VERILATOR) -Wall -Wno-UNUSED --cc --exe --build -j \
		-CFLAGS "$(CXXFLAGS)" \
		-LDFLAGS "$(LDFLAGS)" \
		../generated/MatrixMultiplier.v matrix_multiplier_sim.cpp

clean:
	rm -rf obj_dir

.PHONY: all clean