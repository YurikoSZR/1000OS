/* custom_libc.h */
#ifndef CUSTOM_LIBC_H
#define CUSTOM_LIBC_H

#include <stdarg.h> // For va_list, va_start, va_arg, va_end
#include <stddef.h> // For size_t
#include <stdlib.h>
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
