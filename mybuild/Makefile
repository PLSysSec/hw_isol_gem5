.PHONY: build test

.DEFAULT_GOAL := build

PARALLEL_COUNT=$(shell nproc)
EXAMPLE_FLAGS=-O0 -g
GEM5_TRACE_FLAG=
# GEM5_TRACE_FLAG=--debug-flags=Exec
# GEM5_TRACE_FLAG=--debug-break=1355932000
RUN_SIM=./build/X86/gem5.opt $(GEM5_TRACE_FLAG) configs/example/se.py --cpu-type=DerivO3CPU --caches -c
EXPECT_FAIL=./mybuild/expectfail.sh

build_gem5:
	cd .. && scons build/X86/gem5.opt -j $(PARALLEL_COUNT)

mybuildout/test-umov: ../tests/test-progs/hello/src/test-umov.c
	mkdir -p mybuildout
	$(CC) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi.cpp ../tests/test-progs/hfi/hfi_load_store.S
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi-reenter-fail: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi-reenter-fail.cpp
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi-noenterexit-fail: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi-noenterexit-fail.cpp
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi-oob-fail: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi-oob-fail.cpp ../tests/test-progs/hfi/hfi_load_store.S
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi-readperm-fail: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi-readperm-fail.cpp ../tests/test-progs/hfi/hfi_load_store.S
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

mybuildout/test-hfi-writeperm-fail: ../tests/test-progs/hfi/hfi.S ../tests/test-progs/hfi/test-hfi-writeperm-fail.cpp ../tests/test-progs/hfi/hfi_load_store.S
	mkdir -p mybuildout
	$(CXX) $^ $(EXAMPLE_FLAGS) -o $@

build-tests: mybuildout/test-umov
build-tests: mybuildout/test-hfi
build-tests: mybuildout/test-hfi-reenter-fail
build-tests: mybuildout/test-hfi-noenterexit-fail
build-tests: mybuildout/test-hfi-oob-fail
build-tests: mybuildout/test-hfi-readperm-fail
build-tests: mybuildout/test-hfi-writeperm-fail

build: build_gem5 build-tests

test-umov: mybuildout/test-umov
	cd .. && $(RUN_SIM) mybuild/mybuildout/test-umov -o 1

test-umov-fail: mybuildout/test-umov
	cd .. &&  $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-umov -o 3

test-hfi: mybuildout/test-hfi
	cd .. && $(RUN_SIM) mybuild/mybuildout/test-hfi

test-hfi-reenter-fail: mybuildout/test-hfi-reenter-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-hfi-reenter-fail

test-hfi-noenterexit-fail: mybuildout/test-hfi-noenterexit-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-hfi-noenterexit-fail

test-hfi-oob-fail: mybuildout/test-hfi-oob-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-hfi-oob-fail

test-hfi-readperm-fail: mybuildout/test-hfi-readperm-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-hfi-readperm-fail

test-hfi-writeperm-fail: mybuildout/test-hfi-writeperm-fail
	cd .. && $(EXPECT_FAIL) $(RUN_SIM) mybuild/mybuildout/test-hfi-writeperm-fail

old-test: test-umov test-umov-fail
	@echo "!!!!!!!!!Tests successfully completed!!!!!!!!!"

test: test-hfi test-hfi-reenter-fail test-hfi-noenterexit-fail test-hfi-oob-fail test-hfi-readperm-fail test-hfi-writeperm-fail
	@echo "!!!!!!!!!Tests successfully completed!!!!!!!!!"
