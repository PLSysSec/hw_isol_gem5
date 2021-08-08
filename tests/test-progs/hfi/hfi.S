.text

.globl hfi_get_version
.type hfi_get_version,@function
.align 16
hfi_get_version:
_hfi_get_version:
    # output is already in rax
    .byte   0x0F, 0x04, 0x63, 0x00
    ret

.globl hfi_get_linear_range_count
.type hfi_get_linear_range_count,@function
.align 16
hfi_get_linear_range_count:
_hfi_get_linear_range_count:
    # output is already in rax
    .byte   0x0F, 0x04, 0x64, 0x00
    ret

.globl hfi_set_sandbox_metadata
.type hfi_set_sandbox_metadata,@function
.align 16
hfi_set_sandbox_metadata:
_hfi_set_sandbox_metadata:
    # mov param into rax
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00
    ret

.globl hfi_get_sandbox_metadata
.type hfi_get_sandbox_metadata,@function
.align 16
hfi_get_sandbox_metadata:
_hfi_get_sandbox_metadata:
    # mov param into rax
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x72, 0x00
    ret

.globl hfi_enter_sandbox
.type hfi_enter_sandbox,@function
.align 16
hfi_enter_sandbox:
_hfi_enter_sandbox:
    .byte   0x0F, 0x04, 0x65, 0x00
    ret

.globl hfi_exit_sandbox
.type hfi_exit_sandbox,@function
.align 16
hfi_exit_sandbox:
_hfi_exit_sandbox:
    .byte   0x0F, 0x04, 0x66, 0x00
    ret

.globl hfi_get_exit_reason
.type hfi_get_exit_reason,@function
.align 16
hfi_get_exit_reason:
_hfi_get_exit_reason:
    .byte   0x0F, 0x04, 0x69, 0x00
    ret

.globl hfi_get_exit_location
.type hfi_get_exit_location,@function
.align 16
hfi_get_exit_location:
_hfi_get_exit_location:
    .byte   0x0F, 0x04, 0x70, 0x00
    ret

.globl hfi_save_thread_context
.type hfi_save_thread_context,@function
.align 16
hfi_save_thread_context:
_hfi_save_thread_context:
    # mov param into rax
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x67, 0x00
    ret


.globl hfi_load_thread_context
.type hfi_load_thread_context,@function
.align 16
hfi_load_thread_context:
_hfi_load_thread_context:
    # mov param into rax
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x68, 0x00
    ret

hfi_urmov_load:
    .byte 0x0e # unrestricted mov prefix
    mov  0x0(%rdi), %rax
    ret

hfi_urmov_store:
    .byte 0x0e # unrestricted mov prefix
    mov  %rdi, 0x0(%rsi)
    ret
