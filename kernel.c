#include "kernel.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;
extern char __bss[],__bss_end[],__stack_top[];
__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void){
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" //set the stack pointer
        "j kernel_main\n" //jump to kernel_main
        :
        : [stack_top] "r" (__stack_top)
    );
}

