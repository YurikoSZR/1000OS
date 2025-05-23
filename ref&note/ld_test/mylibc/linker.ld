ENTRY(_custom_start) /* Sets the entry point of our program */

SECTIONS
{
    /* Start address of the program. 0x400000 is a common default for Linux. */
    . = 0x400000;

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
