#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "hfi.h"

typedef void(*void_void_ptr_t)();

void noop_func();
void hfi_call_test(hfi_sandbox* sandbox, void_void_ptr_t* call_slot);

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    memset(&sandbox, 0, sizeof(hfi_sandbox));

    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
    }

    // check call indirect which does a memory reference
    void_void_ptr_t func_ptr = &noop_func;
    hfi_call_test(&sandbox, &func_ptr);
}