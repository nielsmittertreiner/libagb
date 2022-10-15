#ifndef GUARD_AGB_BIOS_DECOMPRESS_H
#define GUARD_AGB_BIOS_DECOMPRESS_H

#include "agb.h"

typedef enum compression_type
{
	COMPRESSION_NONE,
	COMPRESSION_LZ77,
	COMPRESSION_RL,
	COMPRESSION_HUFF,
} compression_type;

typedef struct bit_unpack_param
{
    uint16_t src_size;
	uint8_t src_bits;
	uint8_t dst_bits;
	uint32_t dst_offset:31;
	uint32_t zero_data_flag:1;
} bit_unpack_param;

extern void bit_unpack(const void *src, void *dst, bit_unpack_param *param);
extern void lz77_uncomp_wram(const void *src, void *dst);
extern void lz77_uncomp_vram(const void *src, void *dst);
extern void huff_uncomp(const void *src, void *dst);
extern void rl_uncomp_wram(const void *src, void *dst);
extern void rl_uncomp_vram(const void *src, void *dst);
extern void diff_8bit_unfilter_wram(const void *src, void *dst);
extern void diff_8bit_unfilter_vram(const void *src, void *dst);
extern void diff_16bit_unfilter(const void *src, void *dst);

#endif // GUARD_AGB_BIOS_DECOMPRESS_H