.text

.globl hfi_get_version
.type hfi_get_version,@function
.align 16
hfi_get_version:
_hfi_get_version:
    # output is already in rax
    .byte   0x0F, 0x04, 0x63, 0x00
    ret
