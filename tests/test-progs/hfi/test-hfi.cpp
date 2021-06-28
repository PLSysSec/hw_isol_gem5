#include <assert.h>
#include <iostream>
#include <limits>
#include <cstring>

#include "hfi.h"

extern "C" {
    void hfi_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
    void hfi_load_store_push_pop_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
    uint64_t hfi_load_test(hfi_sandbox* sandbox, void* load_address);
    uint64_t hfi_store_test(hfi_sandbox* sandbox, void* store_address, uint64_t store_value);
    void hfi_loadsavecontext_test(hfi_sandbox* sandbox, hfi_thread_context* save_context,
        hfi_thread_context* load_context, hfi_thread_context* save_context2);
}

int main(int argc, char* argv[])
{
    uint64_t version = hfi_get_version();
    std::cout << "HFI version: " << version << "\n";
    assert(version >= 1);

    uint64_t hfi_linear_range_count = hfi_get_linear_range_count();
    std::cout << "HFI linear range count: " << hfi_linear_range_count << "\n";
    assert(hfi_linear_range_count >= 4);

    hfi_sandbox sandbox;
    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
        sandbox.ranges[i].base_address = 0;
        sandbox.ranges[i].lower_bound = 0;
        sandbox.ranges[i].upper_bound = std::numeric_limits<uint64_t>::max();
    }

    // check entry and exit
    hfi_enter_sandbox(&sandbox);
    hfi_exit_sandbox();

    uint64_t array[] = {0,1,2,3,4,5,6,7};

    //setup sandbox for this array
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].upper_bound = 0;
    }
    sandbox.ranges[0].lower_bound = (uintptr_t) array;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(array[8]);

    // check load and store
    hfi_load_store_test(&sandbox, &(array[3]), &(array[4]));
    assert(array[4] == array[3]);
    array[4] = 4;

    // check load and store with a push pop
    // hfi_load_store_push_pop_test(&sandbox, &(array[3]), &(array[4]));
    // assert(array[4] == array[3]);
    // array[4] = 4;

    // This test saves the current context, loads the target context and then saves the current context again
    // The test plan here is to enter a sandbox
    // 1) save context and see that it has the expected value
    // 2) load context which is the same as the sandbox context, save it again and see if it has the expected value
    hfi_thread_context contexts[3];
    std::memset(contexts, 0, sizeof(hfi_thread_context) * 3);

    hfi_thread_context* save_context  = &(contexts[0]);
    hfi_thread_context* load_context  = &(contexts[0]);
    hfi_thread_context* save_context2 = &(contexts[0]);
    sandbox.ranges[0].lower_bound = (uintptr_t) contexts;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(contexts[4]);

    load_context->curr_sandbox_data = sandbox;
    load_context->inside_sandbox = 1;
    hfi_loadsavecontext_test(&sandbox, save_context, load_context, save_context2);

    assert(std::memcmp(save_context, load_context, sizeof(hfi_thread_context)) == 0);
    assert(std::memcmp(save_context2, load_context, sizeof(hfi_thread_context)) == 0);
}