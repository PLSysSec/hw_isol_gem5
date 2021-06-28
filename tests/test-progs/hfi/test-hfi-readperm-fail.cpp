#include <assert.h>
#include <iostream>
#include <limits>

#include "hfi.h"

extern "C" {
    void hfi_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
    uint64_t hfi_load_test(hfi_sandbox* sandbox, void* load_address);
    void hfi_store_test(hfi_sandbox* sandbox, void* store_address, uint64_t store_value);
}

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 0;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
        sandbox.ranges[i].base_address = 0;
        sandbox.ranges[i].lower_bound = 0;
        sandbox.ranges[i].upper_bound = 0;
    }

    uint64_t array[] = {1,2,3,4,5,6,7,8};

    //setup sandbox for this array
    sandbox.ranges[0].lower_bound = (uintptr_t) array;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(array[8]);

    // check load
    uint64_t a = hfi_load_test(&sandbox, &(array[3]));
    std::cout << a << "\n";
}