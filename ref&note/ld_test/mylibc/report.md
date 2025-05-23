````
/* custom_libc.h */
// This file remains the same as in the "Custom printf and Entry Point (x86-64 Linux)" example.
#ifndef CUSTOM_LIBC_H
#define CUSTOM_LIBC_H

#include <stdarg.h> // For va_list, va_start, va_arg, va_end
#include <stddef.h> // For size_t

// Syscall numbers for x86-64 Linux
#define SYS_WRITE 1
#define SYS_EXIT 60

/**
 * my_write - Writes data to a file descriptor.
 * @fd: File descriptor (1 for stdout).
 * @buf: Pointer to the buffer containing data to write.
 * @count: Number of bytes to write.
 * @return: Number of bytes written on success, -1 on error.
 */
long my_write(int fd, const void *buf, size_t count);

/**
 * my_exit - Terminates the current process.
 * @status: Exit status code.
 * This function does not return.
 */
void my_exit(int status) __attribute__((noreturn));

/**
 * my_strlen - Calculates the length of a string.
 * @s: The null-terminated string.
 * @return: The length of the string (excluding the null terminator).
 */
size_t my_strlen(const char *s);

/**
 * my_printf - A simplified custom printf function.
 * Supports: %s, %d, %x, %c, %%
 * @format: The format string.
 * @...: Variable arguments.
 * @return: The number of characters printed.
 */
int my_printf(const char *format, ...);

#endif /* CUSTOM_LIBC_H */
````
```c
/* syscalls_lib.c */
// Content is identical to syscalls.c from the previous example.
// Will be compiled with -fPIC for the shared library.
// gcc -c syscalls_lib.c -o syscalls_lib.o -fPIC -ffreestanding -masm=intel -I. -g

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
```c
/* my_printf_lib.c */
// Content is identical to my_printf.c from the previous example.
// Will be compiled with -fPIC for the shared library.
// gcc -c my_printf_lib.c -o my_printf_lib.o -fPIC -ffreestanding -masm=intel -I. -g

#include "custom_libc.h"

// Helper to print a single character
static int print_char(char c) {
    return my_write(1, &c, 1); // 1 is stdout
}

// Helper to print a string
static int print_string(const char *s) {
    if (!s) {
        return my_write(1, "(null)", 6);
    }
    return my_write(1, s, my_strlen(s));
}

// Helper to print an integer (decimal or hex)
// Returns number of characters printed
static int print_integer(long long n, int base, int is_signed) {
    char buffer[65]; // Max 64 bits + sign + null terminator
    char *digits = "0123456789abcdef";
    int i = 0;
    int count = 0;
    int sign = 0;

    if (is_signed && n < 0) {
        sign = 1;
        n = -n; // Make n positive for conversion
    } else if (n == 0) {
        buffer[i++] = '0';
        count++;
    }


    if (n == 0 && count == 0) { // Handle case where n is 0 and not already printed
        buffer[i++] = '0';
    } else {
        unsigned long long un = n; // Use unsigned for bitwise operations if needed and for general conversion
        while (un > 0) {
            buffer[i++] = digits[un % base];
            un /= base;
        }
    }


    if (sign) {
        buffer[i++] = '-';
    }

    // Reverse the buffer and print
    int chars_printed = 0;
    for (int j = i - 1; j >= 0; j--) {
        if (print_char(buffer[j]) > 0) {
            chars_printed++;
        } else {
            return -1; // Error in printing
        }
    }
    return chars_printed;
}


int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int chars_printed = 0;
    char c;

    while ((c = *format++)) {
        if (c == '%') {
            c = *format++;
            switch (c) {
                case 's': {
                    char *s_arg = va_arg(args, char *);
                    int res = print_string(s_arg);
                    if (res < 0) return -1;
                    chars_printed += res;
                    break;
                }
                case 'd': {
                    int d_arg = va_arg(args, int);
                    int res = print_integer(d_arg, 10, 1);
                    if (res < 0) return -1;
                    chars_printed += res;
                    break;
                }
                case 'x': {
                    unsigned int x_arg = va_arg(args, unsigned int);
                    int res = print_integer(x_arg, 16, 0);
                    if (res < 0) return -1;
                    chars_printed += res;
                    break;
                }
                case 'c': {
                    char c_arg = (char)va_arg(args, int); // char promotes to int in va_arg
                    if (print_char(c_arg) < 0) return -1;
                    chars_printed++;
                    break;
                }
                case '%': {
                    if (print_char('%') < 0) return -1;
                    chars_printed++;
                    break;
                }
                default:
                    // Unsupported format specifier, print literally
                    if (print_char('%') < 0) return -1;
                    if (print_char(c) < 0) return -1;
                    chars_printed += 2;
                    break;
            }
        } else {
            if (print_char(c) < 0) return -1;
            chars_printed++;
        }
    }

    va_end(args);
    return chars_printed;
}
```c
/* main_dyn.c */
// Content is identical to main.c from the previous "Custom printf and Entry Point" example.
// It includes "custom_libc.h" and expects functions like my_printf to be available.
// gcc -c main_dyn.c -o main_dyn.o -ffreestanding -masm=intel -I. -g

#include "custom_libc.h"
#include <stdint.h> // For uintptr_t

void hello_world() {
    my_printf("Hello, World from custom printf (dynamically linked)!\n");
    my_printf("Let's test some formats:\n");
    my_printf("String: %s\n", "This is a test string.");
    my_printf("Character: %c\n", 'X');
    my_printf("Decimal (positive): %d\n", 12345);
    my_printf("Decimal (negative): %d\n", -6789);
    my_printf("Decimal (zero): %d\n", 0);
    my_printf("Hexadecimal (positive): %x\n", 0xABCDEF);
    my_printf("Hexadecimal (zero): %x\n", 0x0);
    my_printf("A literal percent sign: %%\n");
    my_printf("Pointer address (as hex): %x\n", (unsigned int)(uintptr_t)hello_world); // Example
    my_printf("Testing edge case for integer 0: %d and %x\n", 0, 0);
}

// Our custom entry point
void _custom_start() {
    // No C runtime initialization (argc, argv, envp are not available here)
    // No standard library functions available unless we implement them or link them.

    hello_world();

    my_exit(0); // Exit with status 0 (success)
}
````

````
/* linker_main.ld */
/* Linker script for the main executable, similar to the previous one. */
/* ld -o custom_hello_dyn main_dyn.o -T linker_main.ld -L. -lcustomlibc */

ENTRY(_custom_start) /* Sets the entry point of our program */

SECTIONS
{
    /* Start address of the program. 0x400000 is a common default for Linux. */
    . = 0x400000;

    /* Program headers for dynamic linking.
       The .interp section tells the kernel which dynamic linker to use.
       This is usually added by gcc when linking dynamically, but if using ld directly
       for the final executable link AND it's dynamic, you might need to handle it.
       However, we'll use gcc for the final link step which handles this.
    */
    .interp : { *(.interp) }

    .text :
    {
        *(.text)          /* All .text sections (code) */
        *(.text.*)
    }

    .rodata :
    {
        *(.rodata)        /* All .rodata sections (read-only data) */
        *(.rodata.*)
    }

    /* Sections needed for dynamic linking, typically populated by the linker */
    .dynamic    : { *(.dynamic) }
    .got        : { *(.got) }       /* Global Offset Table */
    .got.plt    : { *(.got.plt) }   /* GOT entries for PLT */
    .plt        : { *(.plt) }       /* Procedure Linkage Table */
    .plt.got    : { *(.plt.got) }
    .plt.sec    : { *(.plt.sec) }


    .data :
    {
        *(.data)          /* All .data sections (initialized data) */
        *(.data.*)
    }

    .bss :
    {
        *(.bss)           /* All .bss sections (uninitialized data) */
        *(COMMON)         /* Common symbols */
    }

    /* Discard debug sections and other unnecessary sections for this simple example */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
        *(.note.ABI-tag)
    }
}
````

```sh
#!/bin/bash
# build_dyn.sh

# Clean up previous build files
rm -f *.o libcustomlibc.so custom_hello_dyn

# --- Stage 1: Build the shared library (libcustomlibc.so) ---
echo "Building shared library libcustomlibc.so..."

# Compile C files for the library with -fPIC
# -fPIC: Generate Position Independent Code, necessary for shared libraries.
# -ffreestanding: Standard library may not exist (though not strictly needed for library components here, good practice).
# -I.: Add current directory to include path (for custom_libc.h)
# -g: Add debug symbols
echo "Compiling syscalls_lib.c for shared library..."
gcc -c syscalls_lib.c -o syscalls_lib.o -fPIC -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "syscalls_lib.c compilation failed"; exit 1; fi

echo "Compiling my_printf_lib.c for shared library..."
gcc -c my_printf_lib.c -o my_printf_lib.o -fPIC -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "my_printf_lib.c compilation failed"; exit 1; fi

# Link object files into a shared library
# -shared: Create a shared library.
# -o libcustomlibc.so: Output file name (convention: lib<name>.so)
echo "Linking shared library libcustomlibc.so..."
gcc -shared -o libcustomlibc.so syscalls_lib.o my_printf_lib.o
if [ $? -ne 0 ]; then echo "Shared library linking failed"; exit 1; fi

echo "Shared library libcustomlibc.so built successfully."
echo ""

# --- Stage 2: Build the main executable (custom_hello_dyn) ---
echo "Building main executable custom_hello_dyn..."

# Compile main_dyn.c
# -ffreestanding: Because it has its own _custom_start and doesn't use standard startup.
echo "Compiling main_dyn.c..."
gcc -c main_dyn.c -o main_dyn.o -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "main_dyn.c compilation failed"; exit 1; fi

# Link the main executable against our shared library
# We use gcc for linking the final executable as it handles dynamic linking setup better by default
# (e.g., adding .interp section, linking against dynamic linker support).
# -T linker_main.ld: Use our custom linker script for the executable.
# -L.: Add current directory to library search path (for -lcustomlibc).
# -lcustomlibc: Link against libcustomlibc.so (linker searches for lib<name>.so).
# -Wl,-rpath,'$ORIGIN': Embed RPATH so the executable finds the library in its own directory at runtime.
#                      Alternatively, use LD_LIBRARY_PATH. '$ORIGIN' is a special linker variable.
echo "Linking main executable custom_hello_dyn..."
gcc main_dyn.o -o custom_hello_dyn -T linker_main.ld -L. -lcustomlibc -Wl,-rpath,'$ORIGIN' -g
# If you don't want to use RPATH, link like this:
# gcc main_dyn.o -o custom_hello_dyn -T linker_main.ld -L. -lcustomlibc -g
# And then run with: LD_LIBRARY_PATH=. ./custom_hello_dyn

if [ $? -ne 0 ]; then echo "Main executable linking failed"; exit 1; fi

echo "Main executable custom_hello_dyn built successfully."
echo ""

# --- Running the executable ---
echo "Running ./custom_hello_dyn:"
echo "(If you did not use RPATH during linking, you might need: LD_LIBRARY_PATH=. ./custom_hello_dyn)"
./custom_hello_dyn

echo ""
echo "Exit status: $?"
echo "You can inspect the executable and library with:"
echo "  ldd ./custom_hello_dyn       (shows dynamic dependencies, should list libcustomlibc.so)"
echo "  objdump -T ./libcustomlibc.so (shows dynamic symbols in the library)"
echo "  nm -D ./libcustomlibc.so     (also shows dynamic symbols)"
echo "  readelf -d ./custom_hello_dyn (shows dynamic section, NEEDED entry for libcustomlibc.so)"

