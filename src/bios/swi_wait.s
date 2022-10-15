    .section .text
    .code 16

    .global halt
    .thumb_func
halt:
    swi 0x02
    bx lr

    .global stop
    .thumb_func
stop:
    swi 0x03
    bx lr

    .global intr_wait
    .thumb_func
intr_wait:
    swi 0x04
    bx lr

    .global vblank_intr_wait
    .thumb_func
vblank_intr_wait:
    swi 0x05
    bx lr
