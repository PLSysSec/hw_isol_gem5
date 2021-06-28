#include <assert.h>
#include <iostream>
#include <limits>

#include "hfi.h"

using void_void_ptr_t = void(*)();

extern "C" {
    void noop_func();
    void hfi_call_test(hfi_sandbox* sandbox, void_void_ptr_t* call_slot);
}

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
        sandbox.ranges[i].base_address = 0;
        sandbox.ranges[i].lower_bound = 0;
        sandbox.ranges[i].upper_bound = 0;
    }

    // check call indirect which does a memory reference
    void_void_ptr_t func_ptr = &noop_func;
    hfi_call_test(&sandbox, &func_ptr);
}