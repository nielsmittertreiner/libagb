#ifndef GUARD_AGB_BIOS_CHECKSUM_H
#define GUARD_AGB_BIOS_CHECKSUM_H

#include "agb.h"

#define BIOS_CHECKSUM_GBA 0xBAAE187F
#define BIOS_CHECKSUM_NDS 0xBAAE1880

extern uint32_t bios_checksum(void);

#endif // GUARD_AGB_BIOS_CHECKSUM_H