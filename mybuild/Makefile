.PHONY: build test clean

.DEFAULT_GOAL := build

PARALLEL_COUNT=$(shell nproc)
EXAMPLE_FLAGS=-O0 -g
GEM5_TRACE_FLAG=
# GEM5_TRACE_FLAG=--debug-flags=Exec
# GEM5_TRACE_FLAG=--debug-flags=HFI
# GEM5_TRACE_FLAG=--debug-break=1355932000
RUN_SIM=./build/X86/gem5.opt $(GEM5_TRACE_FLAG) configs/example/se.py --cpu-type=DerivO3CPU --caches -c
EXPECT_FAIL=./mybuild/expectfail.sh
TEST_FILES := $(wildcard ../tests/test-progs/hfi/test-*.cpp)
TEST_NAMES := $(basename $(notdir $(TEST_FILES)))

build-gem5:
	cd .. && scons build/X86/gem5.opt -j $(PARALLEL_COUNT)

.PRECIOUS: mybuildout/test-%
mybuildout/test-%: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-%.cpp ../tests/test-progs/hfi/hfi_load_store.S
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

build-tests: $(addprefix mybuildout/,$(TEST_NAMES))

test-%-success: mybuildout/test-%-success
	cd .. && $(RUN_SIM) mybuild/$^

test-%-fail: mybuildout/test-%-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/$^

build: build-gem5 build-tests

test: $(TEST_NAMES)
	@echo "!!!!!!!!!Tests successfully completed!!!!!!!!!"

clean-gem5:
	cd .. && scons -c

clean-examples:
	-rm -rf mybuildout

clean: clean-gem5 clean-examples
