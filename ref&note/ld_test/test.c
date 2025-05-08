// hello.c
#include <unistd.h>

void _start() {
    // "Hello, World!\n"
    const char *message = "Hello, World!\n";
    size_t length = 14;

    // syscall: write
    // rax = syscall number (1 for write)
    // rdi = file descriptor (1 for stdout)
    // rsi = pointer to the message
    // rdx = length of the message
    asm volatile (
        "mov $1, %%rax\n\t"    // syscall number for write
        "mov $1, %%rdi\n\t"    // file descriptor 1 is stdout
        "mov %0, %%rsi\n\t"    // pointer to the message
        "mov %1, %%rdx\n\t"    // length of the message
        "syscall\n\t"          // make the syscall
        :
        : "r"(message), "r"(length)
        : "rax", "rdi", "rsi", "rdx"
    );

    // syscall: exit
    // rax = syscall number (60 for exit)
    // rdi = exit code (0 for success)
    asm volatile (
        "mov $60, %%rax\n\t"   // syscall number for exit
        "xor %%rdi, %%rdi\n\t" // exit code 0
        "syscall\n\t"          // make the syscall
        :
        :
        : "rax", "rdi"
    );
}