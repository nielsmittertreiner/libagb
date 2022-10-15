    .section .text
    .code 16

    .global swi_div
    .thumb_func
swi_div:
    swi 0x06
    bx lr

    .global swi_div_arm
    .thumb_func
swi_div_arm:
    swi 0x07
    bx lr

    .global swi_sqrt
    .thumb_func
swi_sqrt:
    swi 0x08
    bx lr

    .global swi_arc_tan
    .thumb_func
swi_arc_tan:
    swi 0x09
    bx lr

    .global swi_arc_tan_2
    .thumb_func
swi_arc_tan_2:
    swi 0x0A
    bx lr
