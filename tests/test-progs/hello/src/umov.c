#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int unrestricted_mov(char * input) {
    // __asm (
    //     ".byte 0x0F, 0x04, 0x56, 0x00, 0x55, 0xf4"
    // );
    __asm__ __volatile__(
        ".byte 0x0F, 0x04 \n\t"
        "mov  (%0), %%edx" : : "r" (input) : "edx"
    );
}

// int mov(char * input) {
//     asm volatile(
//         "mov %0, %%edx": : "r" (input)
//     );
// }

int main(int argc, char* argv[])
{
    char a = 10;
    // mov(&a);
    unrestricted_mov(&a);
}
