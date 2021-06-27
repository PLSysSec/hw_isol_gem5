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

.globl hfi_enter_sandbox
.type hfi_enter_sandbox,@function
.align 16
hfi_enter_sandbox:
_hfi_enter_sandbox:
    # mov param into rax
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x65, 0x00
    ret

.globl hfi_exit_sandbox
.type hfi_exit_sandbox,@function
.align 16
hfi_exit_sandbox:
_hfi_exit_sandbox:
    .byte   0x0F, 0x04, 0x66, 0x00
    ret
