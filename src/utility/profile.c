#include "agb.h"

void profile_start(void)
{
    REG_TMCNT_L(2) = 0;
    REG_TMCNT_L(3) = 0;

    REG_TMCNT_H(2) = 0;
    REG_TMCNT_H(3) = 0;
    
    REG_TMCNT_H(3) = TIMER_ENABLE | 0x4;
    REG_TMCNT_H(2) = TIMER_1CLK | TIMER_ENABLE;
}

uint32_t profile_stop(void)
{
    REG_TMCNT_H(2) = 0;
    REG_TMCNT_H(3) = 0;

    return REG_TMCNT_L(2) | (REG_TMCNT_L(3) << 16);
}
