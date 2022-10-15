    .section .text
    .code 16

    .global multi_boot
    .thumb_func
multi_boot:
    swi 0x25
    bx lr
