#ifndef GUARD_AGB_BIOS_RESET_H
#define GUARD_AGB_BIOS_RESET_H

#include "agb.h"

typedef enum
{
    RESET_EWRAM         = BIT(0),
    RESET_IWRAM         = BIT(1),
    RESET_PALETTE       = BIT(2),
    RESET_VRAM          = BIT(3),
    RESET_OAM           = BIT(4),
    RESET_SIO_REGS      = BIT(5),
    RESET_SOUND_REGS    = BIT(6),
    RESET_REGS          = BIT(7),
    RESET_ALL           = BITS(8),
} reset_flag;

extern void soft_reset(void);
extern void register_ram_reset(reset_flag flags);

#endif // GUARD_AGB_BIOS_RESET_H