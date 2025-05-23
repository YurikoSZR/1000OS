#include "custom_libc.h"

// Wrapper for the 'write' system call (syscall number 1 on x86-64)
long my_write(int fd, const void *buf, size_t count) {
    long ret;
    // rdi: fd, rsi: buf, rdx: count, rax: syscall_number
    __asm__ volatile (
        "mov rax, %1\n"    // syscall number (SYS_WRITE)
        "mov rdi, %2\n"    // fd
        "mov rsi, %3\n"    // buf
        "mov rdx, %4\n"    // count
        "syscall\n"
        : "=a" (ret)       // output: result in rax
        : "i" (SYS_WRITE), "D" ((long)fd), "S" (buf), "d" (count) // inputs
        : "rcx", "r11", "memory" // clobbered registers
    );
    return ret;
}

// Wrapper for the 'exit' system call (syscall number 60 on x86-64)
void my_exit(int status) {
    // rdi: status, rax: syscall_number
    __asm__ volatile (
        "mov rax, %0\n"    // syscall number (SYS_EXIT)
        "mov rdi, %1\n"    // status
        "syscall\n"
        : // no output
        : "i" (SYS_EXIT), "D" ((long)status) // inputs
        : "rcx", "r11", "memory" // clobbered registers
    );
    // This point should not be reached
    while(1); // Should be noreturn
}

size_t my_strlen(const char *s) {
    size_t len = 0;
    while (s[len]) {
        len++;
    }
    return len;
}
