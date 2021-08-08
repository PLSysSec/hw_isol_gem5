#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "hfi.h"

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    memset(&sandbox, 0, sizeof(hfi_sandbox));

    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
        sandbox.ranges[i].upper_bound = UINT64_MAX;
    }

    hfi_set_sandbox_metadata(&sandbox);
    hfi_enter_sandbox();
    // enter sandbox while in a sandbox should fail
    hfi_enter_sandbox();
}