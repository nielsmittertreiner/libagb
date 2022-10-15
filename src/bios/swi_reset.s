    .section .text
    .code 16

    .global soft_reset
    .thumb_func
soft_reset:
    swi 0x00
    bx lr

    .global register_ram_reset
    .thumb_func
register_ram_reset:
    swi 0x01
    bx lr
