#include "custom_libc.h"

void hello_world() {
    my_printf("Hello, World from custom printf!\n");
    my_printf("Let's test some formats:\n");
    my_printf("String: %s\n", "This is a test string.");
    my_printf("Character: %c\n", 'X');
    my_printf("Decimal (positive): %d\n", 12345);
    my_printf("Decimal (negative): %d\n", -6789);
    my_printf("Decimal (zero): %d\n", 0);
    my_printf("Hexadecimal (positive): %x\n", 0xABCDEF);
    my_printf("Hexadecimal (zero): %x\n", 0x0);
    my_printf("A literal percent sign: %%\n");
    my_printf("Testing edge case for integer 0: %d and %x\n", 0, 0);
}

// Our custom entry point
void _custom_start() {
    // No C runtime initialization (argc, argv, envp are not available here)
    // No standard library functions available unless we implement them.

    hello_world();

    my_exit(0); // Exit with status 0 (success)
}
