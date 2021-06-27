#include <assert.h>
#include <iostream>
#include <limits>

#include "hfi.h"

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].base_address = 0;
        sandbox.ranges[i].lower_bound = 0;
        sandbox.ranges[i].upper_bound = std::numeric_limits<uint64_t>::max();
    }

    hfi_enter_sandbox(&sandbox);
    // enter sandbox while in a sandbox should fail
    hfi_enter_sandbox(&sandbox);
}