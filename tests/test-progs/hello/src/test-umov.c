#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Sandbox
{
    uint64_t id;
    uint64_t* base_1;
    uint64_t size_1;
    uint64_t* base_2;
    uint64_t size_2;
};

int load_range(void* addr) {
    __asm (
        "mov %0, %%rax;\
        .byte 0x0F, 0x04, 0x55, 0x00"
        :
        :"r"(addr)
        :
    );
}

int sand_enable() {
    __asm (
        ".byte 0x0F, 0x04, 0x58, 0x00"
    );
}

int sand_disable() {
    __asm (
        ".byte 0x0F, 0x04, 0x57, 0x00"
    );
}

int unrestricted_mov(char * input) {
    __asm__ __volatile__(
        ".byte 0x0e;  mov  (%0), %%rdx" : : "r" (input) : "rdx"
    );
}

int mov(char * input) {
    asm volatile(
        "mov (%0), %%edx": : "r" (input) :
    );
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Needs one argument for array index\n");
        return -1;
    }
    long arg = strtol(argv[1], NULL, 10);
    uint32_t arr [4] = { 0x11, 0x22, 0x33, 0x44 };

    // Sandbox around arr includes index 0 and 1.
    struct Sandbox sandbox = {1, 0, 0x7fffffffed38, 0, 0x7fffffffed3f};
    printf("arr start address: %" PRIx64 "\n", &arr);
    printf("base: %" PRIx64 "\n", sandbox.base_1);
    printf("size : %" PRIx64 "\n", sandbox.size_1);
    sand_enable();
    load_range((intptr_t)&sandbox);

    mov(arr + arg);
    // unrestricted_mov(arr + arg);
    sand_disable();

    return 0;
}
