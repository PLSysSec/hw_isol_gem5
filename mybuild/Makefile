.PHONY: build

.DEFAULT_GOAL := build

PARALLEL_COUNT=8
EXAMPLE_FLAGS=-O0 -g
# GEM5_TRACE_FLAG=--debug-flags=Exec
# GEM5_TRACE_FLAG=--debug-break=1355932000
GEM5_TRACE_FLAG=
RUN_SIM=./build/X86/gem5.opt $(GEM5_TRACE_FLAG) configs/example/se.py --cpu-type=DerivO3CPU --caches -c

build_gem5:
	cd .. && scons build/X86/gem5.opt -j $(PARALLEL_COUNT)

mybuildout/test-umov: ../tests/test-progs/hello/src/test-umov.c
	$(CC) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi: ../tests/test-progs/hfi/test-hfi.cpp ../tests/test-progs/hfi/hfi.S
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

build-tests: mybuildout/test-umov mybuildout/test-hfi

build: build_gem5 build-tests


test-umov: build-tests
	cd .. && $(RUN_SIM) mybuild/mybuildout/test-umov -o 5
test-hfi: build-tests
	cd .. && $(RUN_SIM) mybuild/mybuildout/test-hfi

test: test-umov test-hfi