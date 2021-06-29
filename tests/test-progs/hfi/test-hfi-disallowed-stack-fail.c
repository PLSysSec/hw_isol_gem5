#include <inttypes.h>
#include <limits.h>
#include <string.h>

#include "hfi.h"

void hfi_load_store_push_pop_test(hfi_sandbox* sandbox, void* load_address, void* store_address);

hfi_sandbox get_full_access_sandbox() {
    hfi_sandbox sandbox;
    memset(&sandbox, 0, sizeof(hfi_sandbox));
    sandbox.ranges[0].readable = 1;
    sandbox.ranges[0].writeable = 1;
    sandbox.ranges[0].executable = 1;
    sandbox.ranges[0].upper_bound = UINT64_MAX;
    return sandbox;
}

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox = get_full_access_sandbox();
    uint64_t array[] = {0,1,2,3,4,5,6,7};
    sandbox.ranges[0].lower_bound = (uintptr_t) array;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(array[5]);
    sandbox.disallow_unrestricted_stack = 1;

    hfi_load_store_push_pop_test(&sandbox, &(array[3]), &(array[4]));
}