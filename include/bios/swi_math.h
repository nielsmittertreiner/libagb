#ifndef GUARD_AGB_BIOS_MATH_H
#define GUARD_AGB_BIOS_MATH_H

#include "agb.h"

extern int32_t swi_div(int32_t num, int32_t den);
extern int32_t swi_div_arm(int32_t den, int32_t num);
extern uint32_t swi_sqrt(uint32_t num);
extern int16_t swi_arc_tan(int16_t tan);
extern int16_t swi_arc_tan_2(int16_t x, int16_t y);

#endif // GUARD_AGB_BIOS_MATH_H