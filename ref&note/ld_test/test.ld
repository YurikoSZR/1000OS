ENTRY(_start)

MEMORY {
    RAM : ORIGIN = 0x10000000, LENGTH = 128M
}

SECTIONS {
    /* Set the base address for the kernel */
    . = ORIGIN(RAM);
    __kernel_base = .;

    /* Text section */
    .text : ALIGN(0x1000) {
        KEEP(*(.text._start))
        *(.text .text.*)
        *(.gnu.warning)
    } > RAM :text

    /* Read-only data section */
    .rodata : ALIGN(0x1000) {
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)
    } > RAM :rodata

    /* Data section */
    .data : ALIGN(0x1000) {
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        CONSTRUCTORS
    } > RAM :data

    /* BSS section */
    .bss : ALIGN(0x1000) {
        __bss = .;
        *(COMMON)
        *(.bss .bss.* .sbss .sbss.*)
        __bss_end = .;
    } > RAM :bss

    /* Kernel stack */
    . = ALIGN(0x1000);
    . += 128 * 1024; /* 128KB for Kernel Stack */
    __stack_top = .;

    /* Heap */
    . = ALIGN(0x1000);
    __free_ram = .;
    . += 64 * 1024 * 1024; /* 64MB for Heap */
    __free_ram_end = .;
}

PHDRS {
    text PT_LOAD FILEHDR PHDRS FLAGS(5); /* PF_X | PF_R */
    rodata PT_LOAD FLAGS(4); /* PF_R */
    data PT_LOAD FLAGS(6); /* PF_X | PF_W */
    bss PT_LOAD FLAGS(6); /* PF_X | PF_W */
}