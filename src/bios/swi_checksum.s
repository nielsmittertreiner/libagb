    .section .text
    .code 16

    .global bios_checksum
    .thumb_func
bios_checksum:
    swi 0x0D
    bx lr
