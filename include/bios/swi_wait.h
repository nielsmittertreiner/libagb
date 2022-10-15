#ifndef GUARD_AGB_BIOS_WAIT_H
#define GUARD_AGB_BIOS_WAIT_H

#include "agb.h"

extern void halt(void);
extern void stop(void);
extern void intr_wait(uint8_t clear, uint16_t mask);
extern void vblank_intr_wait(void);

#endif // GUARD_AGB_BIOS_WAIT_H