#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Range size can be up to 2^48, but the upper bound address must be aligned
    // to 2^16.
    HFI_RANGE_SIZE_TYPE_LARGE,
    // Range size can be up to 2^32, but the range must not cross a 2^32
    // boundary. If you want a small range that cross a 2^32 boundary, split the
    // range into two separate small ranges, that don't cross the boundary.
    HFI_RANGE_SIZE_TYPE_SMALL
} HFI_RANGE_SIZE_TYPE;


/**
 * @brief Metadata for one linear data range of a sandbox
 */
typedef struct {
    /**
     * @brief Read permissions for this range
     */
    uint8_t readable;
    /**
     * @brief Write permissions for this range
     */
    uint8_t writeable;
    /**
     * @brief Range size type from HFI_RANGE_SIZE_TYPE
     */
    uint8_t range_size_type;
    /**
     * @brief base_address --- Used by the trusted sandbox. A constant base whose value is added to all loads and stores
     * performed in this region.
     * base_mask --- Used by the untrusted sandbox. A mask that is uses as a prefix check for all loads and stores
     * performed in this region.
     */
    union {
        // used by the trusted sandbox
        uint64_t base_address;
        // used by the untrusted sandobx
        uint64_t base_mask;
    };
    /**
     * @brief offset_limit --- Used by the trusted sandbox. The size of the
     * region. Must be a power of 64k.
     * ignore_mask --- Used by the untrusted sandbox. A mask that is used to
     * drop unchecked bits of the suffix for all loads and stores performed in
     * this region.
     */
    union {
        // used by the trusted sandbox
        uint64_t offset_limit;
        // used by the untrusted sandobx
        uint64_t ignore_mask;
    };
} hfi_linear_data_range;

/**
 * @brief Metadata for one linear code range of a sandbox
 */
typedef struct {
    /**
     * @brief Execute permissions for this range
     */
    uint8_t executable;
    /**
     * @brief A mask that is uses as a prefix check for all instructions.
     */
    uint64_t base_mask;
    /**
     * @brief A mask that is used to drop unchecked bits of the suffix for all
     * instructions.
     */
    uint64_t ignore_mask;
} hfi_linear_code_range;

/**
 * @brief The number of linear data ranges supported. This can be different on
 * different HFI versions. In this version, it is 4.
 */
#define HFI_LINEAR_DATA_RANGE_COUNT 4

/**
 * @brief The number of linear code ranges supported. This can be different on
 * different HFI versions. In this version, it is 4.
 */
#define HFI_LINEAR_CODE_RANGE_COUNT 2

// The metadata required for a "hardware sandbox"
typedef struct {
    /**
     * @brief Specifies an address range and its associated permissions for data read/write
     */
    hfi_linear_data_range data_ranges[HFI_LINEAR_DATA_RANGE_COUNT];
        /**
     * @brief Specifies an address range and its associated permissions for code exec
     */
    hfi_linear_code_range code_ranges[HFI_LINEAR_CODE_RANGE_COUNT];
    /**
     * @brief Bit that controls whether this is a "structured" sandbox with a
     * trusted compiler.
     *
     * In a structured sandbox, existing instructions are unchanged but binaries
     * get a new hfi sandboxed mov instruction "hmov"
     *
     *  hmov reg_region_num, reg * {1, 2, 4, 8}, disp_32
     *
     * as well as instructions with region number encoded in the instruction
     *
     *  hmov1 reg * {1, 2, 4, 8}, disp_32
     *  hmov2 reg * {1, 2, 4, 8}, disp_32
     *  hmov3 reg * {1, 2, 4, 8}, disp_32
     *  hmov4 reg * {1, 2, 4, 8}, disp_32
     *
     * Structured sandboxes can also use the hfi instructions such as
     * "hfi_set_sandbox_metadata", "hfi_get_sandbox_metadata" etc.
     *
     * In an unstructured sandbox, there are no new instructions, but the
     * existing mov instruction is modified to be sandboxed. Unstructured
     * sandboxes cannot call hfi instructions other than hfi_exit
     */
    bool is_trusted_sandbox;
    /**
     * @brief Optional code address that is called (jumped to) when the
     * hfi_exit_sandbox or syscall instruction is run while inside a sandbox
     */
    void* exit_sandbox_handler;
} hfi_sandbox;

// Context structures that contain all hfi metadata
// Hardware will allocate registers to store this metadata
// This structure will also be used to save context when the OS schedules threads (shown later)
enum HFI_EXIT_REASON {
    HFI_EXIT_REASON_INTERRUPT_0,
    HFI_EXIT_REASON_INTERRUPT_1,
    // .. through 2^8 - 1 = HFI_EXIT_REASON_INTERRUPT_255
    HFI_EXIT_REASON_EXIT = 1024,
    HFI_EXIT_REASON_SYSCALL = 1025,
    HFI_EXIT_REASON_SYSENTER = 1026,
    HFI_EXIT_REASON_PRIVSWITCH = 1027
};
typedef struct {
    hfi_sandbox curr_sandbox_data;
    bool inside_sandbox;
    /* HFI_EXIT_REASON */ uint32_t exit_sandbox_reason;
    void* exit_instruction_pointer;
} hfi_thread_context;

// Get the version of HFI implemented in hardware.
// Return value: the version of the hfi
#define hfi_get_version(out_ret)         \
    asm(".byte 0x0F, 0x04, 0x63, 0x00\n" \
    : "=a"(out_ret)                      \
    )

// Get the number of data ranges supported by HFI.
// Return value: the number of data ranges supported by HFI.
#define hfi_get_linear_data_range_count(out_ret) \
    asm(".byte 0x0F, 0x04, 0x64, 0x00\n"         \
    : "=a"(out_ret)                              \
    )

// Get the number of code ranges supported by HFI.
// Return value: the number of code ranges supported by HFI.
#define hfi_get_linear_code_range_count(out_ret) \
    asm(".byte 0x0F, 0x04, 0x73, 0x00\n"         \
    : "=a"(out_ret)                              \
    )

// Instruction executed to configure the sandbox for the current thread.
// This loads the hfi CPU registers with bounds information used for checking.
// Parameters: the current sandbox's data of type "const hfi_sandbox*"
#define hfi_set_sandbox_metadata(hfi_metadata) \
    asm(".byte 0x0F, 0x04, 0x71, 0x00\n"       \
    :                                          \
    : "a"(hfi_metadata)                        \
    )

// Instruction executed to get the current configuration the sandbox for the current thread.
// This loads the hfi CPU registers with bounds information used for checking.
// Parameters: the current sandbox's data of type "const hfi_sandbox*"
#define hfi_get_sandbox_metadata(out_hfi_metadata) \
    asm(".byte 0x0F, 0x04, 0x72, 0x00\n"           \
    :                                              \
    : "a"(out_hfi_metadata)                        \
    )

// Instruction executed to enter a sandbox.
// This enables the hfi bounds checking.
#define hfi_enter_sandbox() asm(".byte 0x0F, 0x04, 0x65, 0x00\n")

// Instruction executed to exit a sandbox. Can be invoked by any code
// Relies on trusted compilers to ensure this instruction is not misused/called from a bad context
#define hfi_exit_sandbox() asm(".byte 0x0F, 0x04, 0x66, 0x00\n")


// Instruction that gets the last reason for sandbox exit
// Return of type enum HFI_EXIT_REASON
#define hfi_get_exit_reason(out_ret)     \
    asm(".byte 0x0F, 0x04, 0x69, 0x00\n" \
    : "=a"(out_ret)                      \
    )

// Instruction that gets the last reason for sandbox exit
// Return of type void*
#define hfi_get_exit_location(out_ret)   \
    asm(".byte 0x0F, 0x04, 0x70, 0x00\n" \
    : "=a"(out_ret)                      \
    )

////////////////
// HFI mov instructions exposed as functions
////////////////

// Load/store from the given region number, offset

#define hfi_mov_load_anytype(region, offset, data)                               \
    asm(".byte 0x0e\n"                                                           \
    "mov (%1, %2), %0\n"                                                         \
    : "=r"(data)                                                                 \
    : "r"(region), "r"(offset)                                                   \
    );

#define hfi_mov_store_anytype(region, offset, data)                              \
    asm(".byte 0x0e\n"                                                           \
    "mov %0, (%1, %2)\n"                                                         \
    :                                                                            \
    : "r"(data), "r"(region), "r"(offset)                                        \
    );

// Load/store from the region 1, offset
#define hfi_mov1_load_anytype(offset, data)                 \
    asm(".byte 0x0d\n"                                           \
    "mov (%1), %0\n"                                             \
    : "=r"(data)                                                 \
    : "r"(offset)                                                \
    );

#define hfi_mov1_store_anytype(offset, data)                     \
    asm(".byte 0x0d\n"                                           \
    "mov %0, (%1)\n"                                             \
    :                                                            \
    : "r"(data), "r"(offset)                                     \
    );

#ifdef __cplusplus
}
#endif
