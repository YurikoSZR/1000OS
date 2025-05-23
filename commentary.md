# Start QEMU, the emulator/virtualizer.
# $QEMU is likely a variable holding the path to the QEMU executable (e.g., qemu-system-aarch64).
````
$QEMU \
    # Specifies the virtual machine's architecture and features.
    # 'virt' is a generic machine type often used for emulating ARM boards,
    # providing a minimal set of devices suitable for running kernels.
    -machine virt \
    # Uses QEMU's default firmware/BIOS. This is usually a built-in firmware
    # like OVMF (for x86) or an ARM-specific firmware like U-Boot/EDK2 for 'virt' machine.
    -bios default \
    # Disables QEMU's graphical output. The VM will run without a visible window.
    # All interaction and output will typically go through the serial console.
    -nographic \
    # Redirects the primary serial port and QEMU's monitor to standard I/O (your terminal).
    # 'mon:stdio' means you can type QEMU monitor commands directly in your terminal,
    # and guest output will also appear there.
    -serial mon:stdio \
    # Prevents QEMU from automatically rebooting the guest system if it tries to reboot.
    # Instead, QEMU will exit. Useful for single-run tests or debugging.
    --no-reboot \
    # Specifies debugging options. These flags tell QEMU to print specific debug messages:
    # - 'unimp': Unimplemented instructions/features.
    # - 'guest_errors': Errors originating from the guest OS.
    # - 'int': Interrupt-related events.
    # - 'cpu_reset': CPU reset events.
    -d unimp,guest_errors,int,cpu_reset \
    # Redirects all debug output (specified by '-d') to a file named 'qemu.log'.
    -D qemu.log \
    # Defines a disk drive for the virtual machine.
    # - 'id=drive0': Assigns a unique ID to this drive for internal reference.
    # - 'file=disk.tar': Specifies the path to the disk image file.
    # - 'format=raw': Tells QEMU to interpret the file as a raw disk image (byte-for-byte copy).
    # - 'if=none': Means this drive is *not* connected to any specific interface yet;
    #              it needs to be explicitly attached with a '-device' option.
    -drive id=drive0,file=disk.tar,format=raw,if=none \
    # Connects the previously defined 'drive0' as a virtio-blk-device.
    # - 'virtio-blk-device': A paravirtualized block device that offers high performance.
    # - 'drive=drive0': Links this device to the disk drive defined by 'id=drive0'.
    # - 'bus=virtio-mmio-bus.0': Connects it to a specific virtio memory-mapped I/O bus,
    #                             which is common for ARM 'virt' machines.
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
    # Specifies the kernel executable to load and run directly.
    # QEMU will bypass the normal boot process (BIOS/bootloader) and jump straight
    # into the 'kernel.elf' file, assuming it's a valid kernel image for the emulated architecture.
    -kernel kernel.elf
````