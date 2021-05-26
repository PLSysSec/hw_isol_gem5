#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Sandbox
{
    uint64_t id;
    uint64_t* base_1;
    uint64_t size_1;
    uint64_t* base_2;
    uint64_t size_2
};

int entry(void* addr) {
    __asm (
        "mov %0, %%rax;\
        .byte 0x0F, 0x04, 0x55, 0x00"
        :
        :"r"(addr)
        :
    );
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Needs one argument for array length\n");
        return -1;
    }
    long arg = strtol(argv[1], NULL, 10);
    int arr [4] = { 16, 2, 77, 40 };
    // struct Sandbox sandbox = {55, &arr[0], 4*sizeof(int)};
    struct Sandbox sandbox = {1, 0, 0x7fffffffefff, 0, 0x7fffffffecd8};
    printf("%s\n", argv[1]);
    printf("sandbox start address %x\n", &sandbox);
    printf("base %x\n", sandbox.base_1);
    printf("base+size %x\n", sandbox.base_1 + sandbox.size_1);
    entry((intptr_t)&sandbox);

    int a = arr[arg];
    return 0;
}
