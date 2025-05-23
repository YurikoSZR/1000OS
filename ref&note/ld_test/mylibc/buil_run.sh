#!/bin/bash
# build_and_run.sh

# Clean up previous build files
rm -f *.o custom_hello

# Compile C files
# -ffreestanding: Implies that the standard library may not exist.
# -fno-builtin: Don't recognize built-in functions that have special handling.
# -masm=intel: Use Intel assembly syntax for inline asm (optional, but often preferred)
# -I.: Add current directory to include path (for custom_libc.h)
# -g: Add debug symbols (optional, useful for gdb)
# -c: Compile only, do not link yet
echo "Compiling syscalls.c..."
gcc -c syscalls.c -o syscalls.o -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "syscalls.c compilation failed"; exit 1; fi

echo "Compiling my_printf.c..."
gcc -c my_printf.c -o my_printf.o -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "my_printf.c compilation failed"; exit 1; fi

echo "Compiling main.c..."
gcc -c main.c -o main.o -ffreestanding -masm=intel -I. -g
if [ $? -ne 0 ]; then echo "main.c compilation failed"; exit 1; fi

# Link object files using our custom linker script
# -nostdlib: Do not link with standard system startup files or libraries.
# -T linker.ld: Use our custom linker script.
echo "Linking..."
ld -o custom_hello syscalls.o my_printf.o main.o -T linker.ld
if [ $? -ne 0 ]; then echo "Linking failed"; exit 1; fi

echo "Build successful: ./custom_hello"
echo "Running ./custom_hello:"
./custom_hello

echo ""
echo "Exit status: $?"
echo "You can inspect the executable with:"
echo "  objdump -d ./custom_hello  (disassembly)"
echo "  objdump -h ./custom_hello  (section headers)"
echo "  readelf -l ./custom_hello (program headers)"
echo "  nm ./custom_hello         (symbols)"
