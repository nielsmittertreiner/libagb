    .section .text
    .code 16

    .global cpu_set
    .thumb_func
cpu_set:
    swi 0x0B
    bx lr

    .global cpu_fast_set
    .thumb_func
cpu_fast_set:
    swi 0x0C
    bx lr
