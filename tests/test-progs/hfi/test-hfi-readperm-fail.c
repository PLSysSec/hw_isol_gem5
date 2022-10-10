#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "hfi.h"

void hfi_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
uint64_t hfi_load_test(hfi_sandbox* sandbox, void* load_address);
void hfi_store_test(hfi_sandbox* sandbox, void* store_address, uint64_t store_value);

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    memset(&sandbox, 0, sizeof(hfi_sandbox));

    // initialize ranges
    for(uint64_t i = 0; i < HFI_LINEAR_DATA_RANGE_COUNT; i++) {
        sandbox.data_ranges[i].readable = 0;
        sandbox.data_ranges[i].writeable = 1;
    }

    uint64_t array[] = {1,2,3,4,5,6,7,8};

    //setup sandbox for this array
    sandbox.data_ranges[0].lower_bound = (uintptr_t) array;
    sandbox.data_ranges[0].upper_bound = (uintptr_t) &(array[8]);

    // check load
    uint64_t a = hfi_load_test(&sandbox, &(array[3]));
    printf("%"PRIu64"\n", a);
}