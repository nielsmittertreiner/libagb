    .section .text
    .code 16

    .global bit_unpack
    .thumb_func
bit_unpack:
    swi 0x10
    bx lr

    .global lz77_uncomp_wram
    .thumb_func
lz77_uncomp_wram:
    swi 0x11
    bx lr

    .global lz77_uncomp_vram
    .thumb_func
lz77_uncomp_vram:
    swi 0x12
    bx lr

    .global huff_uncomp
    .thumb_func
huff_uncomp:
    swi 0x13
    bx lr

    .global rl_uncomp_wram
    .thumb_func
rl_uncomp_wram:
    swi 0x14
    bx lr

    .global rl_uncomp_vram
    .thumb_func
rl_uncomp_vram:
    swi 0x15
    bx lr

    .global diff_8bit_unfilter_wram
    .thumb_func
diff_8bit_unfilter_wram:
    swi 0x16
    bx lr

    .global diff_8bit_unfilter_vram
    .thumb_func
diff_8bit_unfilter_vram:
    swi 0x17
    bx lr

    .global diff_16bit_unfilter
    .thumb_func
diff_16bit_unfilter:
    swi 0x18
    bx lr
