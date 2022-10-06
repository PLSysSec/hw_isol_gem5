#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "hfi.h"
#include "test-common.h"

typedef void(*void_void_ptr_t)();

void hfi_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
void hfi_load_store_ret_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
uint64_t hfi_load_test(hfi_sandbox* sandbox, void* load_address);
void hfi_store_test(hfi_sandbox* sandbox, void* store_address, uint64_t store_value);
void noop_func();
void hfi_loadsavecontext_test(hfi_sandbox* sandbox, hfi_thread_context* save_context,
    hfi_thread_context* load_context, hfi_thread_context* save_context2);
void hfi_exit_handler_test(hfi_sandbox* sandbox);
void hfi_test_exit_location();

void* assert_memcmp(const void* p1, const void* p2, size_t n) {
    bool is_equal = memcmp(p1, p2, n) == 0;
    if (!is_equal) {
        const char* c1 = (const char*) p1;
        const char* c2 = (const char*) p2;
        for(size_t i = 0; i < n; i++) {
            const char* diff_string = c1[i] == c2[i]? "" : "<----";
            printf("Byte %zu: (left, right): %02X, %02X %s\n", i, c1[i], c2[i], diff_string); fflush(stdout);
        }
        assert(is_equal);
    }
}

void test_entry_exit() {
    hfi_sandbox sandbox = get_full_access_mask_sandbox();
    printf("test_entry_exit\n"); fflush(stdout);
    hfi_set_sandbox_metadata(&sandbox);
    hfi_enter_sandbox();
    hfi_exit_sandbox();
}

void test_load_store() {
    hfi_sandbox sandbox = get_full_access_mask_sandbox();
    uint64_t array[] = {0,1,2,3,4,5,6,7};
    sandbox.data_ranges[0].lower_bound = (uintptr_t) array;
    sandbox.data_ranges[0].upper_bound = (uintptr_t) &(array[5]);

    // check load and store
    printf("test_load_store\n"); fflush(stdout);
    hfi_load_store_test(&sandbox, &(array[3]), &(array[4]));
    assert(array[4] == array[3]);
    array[4] = 4;
}

void test_load_store_with_base() {
    hfi_sandbox sandbox = get_full_access_mask_sandbox();
    uint64_t array[] = {0,1,2,3,4,5,6,7};

    // check load and store with a base address
    sandbox.data_ranges[0].base_address = (uintptr_t) array;
    sandbox.data_ranges[0].lower_bound = 0;
    sandbox.data_ranges[0].upper_bound = ((uintptr_t) &(array[5])) - ((uintptr_t) &(array[0]));
    printf("test_load_store_with_base\n"); fflush(stdout);
    hfi_load_store_test(&sandbox,
        (void*) (((uintptr_t) &(array[3])) - ((uintptr_t) &(array[0]))),
        (void*) (((uintptr_t) &(array[4])) - ((uintptr_t) &(array[0])))
    );
    assert(array[4] == array[3]);
    array[4] = 4;
}

void test_save_load_context() {
    hfi_sandbox sandbox = get_full_access_mask_sandbox();

    // This test saves the current context, loads the target context and then saves the current context again
    // The test plan here is to enter a sandbox
    // 1) save context and see that it has the expected value
    // 2) load context which is the same as the sandbox context, save it again and see if it has the expected value
    hfi_thread_context contexts[3];
    memset(contexts, 0, sizeof(hfi_thread_context) * 3);

    hfi_thread_context* save_context  = &(contexts[0]);
    hfi_thread_context* load_context  = &(contexts[1]);
    hfi_thread_context* save_context2 = &(contexts[2]);
    sandbox.data_ranges[0].lower_bound = (uintptr_t) contexts;
    sandbox.data_ranges[0].upper_bound = (uintptr_t) &(contexts[4]);

    load_context->curr_sandbox_data = sandbox;
    load_context->inside_sandbox = 1;
    load_context->exit_sandbox_reason = hfi_get_exit_reason();
    load_context->exit_instruction_pointer = hfi_get_exit_location();
    printf("test_save_load_context\n"); fflush(stdout);
    hfi_loadsavecontext_test(&sandbox, save_context, load_context, save_context2);

    assert_memcmp(save_context, load_context, sizeof(hfi_thread_context));
    assert_memcmp(save_context2, load_context, sizeof(hfi_thread_context));
}

void test_exit_handler() {
    hfi_sandbox sandbox = get_full_access_mask_sandbox();
    sandbox.exit_sandbox_handler = (void*) &noop_func;
    printf("test_exit_handler\n"); fflush(stdout);
    hfi_exit_handler_test(&sandbox);
    enum HFI_EXIT_REASON exit_reason = hfi_get_exit_reason();
    assert(exit_reason == HFI_EXIT_REASON_EXIT);
    void* exit_location = hfi_get_exit_location();
    void* expected = &hfi_test_exit_location;
    assert(exit_location == expected);
}

int main(int argc, char* argv[])
{
    uint64_t version = hfi_get_version();
    printf("HFI version: %"PRIu64"\n", version); fflush(stdout);
    assert(version >= 2);

    uint64_t hfi_linear_data_range_count = hfi_get_linear_data_range_count();
    printf("HFI linear data range count: %"PRIu64"\n", hfi_linear_data_range_count); fflush(stdout);
    assert(hfi_linear_data_range_count >= 4);

    test_entry_exit();
    test_load_store();
    test_load_store_with_base();
    test_save_load_context();
    test_save_load_metadata();
    test_exit_handler();
}