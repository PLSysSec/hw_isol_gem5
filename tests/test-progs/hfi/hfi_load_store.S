.text

.globl hfi_load_store_test
.type hfi_load_store_test,@function
.align 16
hfi_load_store_test:
_hfi_load_store_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rcx
    mov %rcx, 0x0(%rdx)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_load_store_push_pop_test
.type hfi_load_store_push_pop_test,@function
.align 16
hfi_load_store_push_pop_test:
_hfi_load_store_push_pop_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, push&pop, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rcx
    push %rcx
    pop %r8
    mov %r8, 0x0(%rdx)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_load_test
.type hfi_load_test,@function
.align 16
hfi_load_test:
_hfi_load_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # enter the sandbox, perform load, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rax
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_store_test
.type hfi_store_test,@function
.align 16
hfi_store_test:
_hfi_store_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the store address
    # rdx: param 3 is the store value
    # enter the sandbox, perform store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov %rdx, 0x0(%rsi)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret
