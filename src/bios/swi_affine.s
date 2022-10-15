    .section .text
    .code 16

    .global bg_affine_set
    .thumb_func
bg_affine_set:
    swi 0x0E
    bx lr

    .global obj_affine_set
    .thumb_func
obj_affine_set:
    swi 0x0F
    bx lr
