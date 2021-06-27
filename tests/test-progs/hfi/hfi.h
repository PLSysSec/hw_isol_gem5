#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Metadata for one linear range or "segment" of a sandbox
struct hfi_linear_range {
    // Permissions for this range
    char readable;
    char writeable;
    char executable;
    // A constant base whose value is added to all loads and stores performed in the sandbox
    uint64_t base_address;
    // The lower bound of this segment's allowed range
    uint64_t lower_bound;
    // The upper bound of this segment's allowed range
    uint64_t upper_bound;
};

// The number of linear ranges supported
// This can be different on different machines
// In this machine, it is 4
#define LINEAR_RANGE_COUNT 4

// The metadata required for a "hardware sandbox"
struct hfi_sandbox {
    // Each segment specifies an address range and its associated permissions
    hfi_linear_range ranges[LINEAR_RANGE_COUNT];
};

// Get the version of HFI implemented in hardware.
// Return value: the version of the hfi
uint64_t hfi_get_version();

// Get the number of ranges supported by HFI.
// Return value: the number of ranges supported by HFI.
uint64_t hfi_get_linear_range_count();

// Instruction executed to enter a sandbox.
// This loads the hfi CPU registers with bounds information used for checking.
// Parameters: the current sandbox's data
void hfi_enter_sandbox(hfi_sandbox* param_hfi_curr_sandbox_data);

// Instruction executed to exit a sandbox. Can be invoked by any code
// Relies on trusted compilers to ensure this instruction is not misused/called from a bad context
void hfi_exit_sandbox();

#ifdef __cplusplus
}
#endif
