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
