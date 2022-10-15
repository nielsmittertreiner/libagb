#include "agb.h"

void benchmark_start(void)
{
    REG_TMCNT_L(2) = 0;
    REG_TMCNT_L(3) = 0;

    REG_TMCNT_H(2) = 0;
    REG_TMCNT_H(3) = 0;
    
    REG_TMCNT_H(3) = TIMER_ENABLE | 0x4;
    REG_TMCNT_H(2) = TIMER_1CLK | TIMER_ENABLE;
}

u32 benchmark_end(void)
{
    REG_TMCNT_H(2) = 0;
    REG_TMCNT_H(3) = 0;

    return REG_TMCNT_L(2) | (REG_TMCNT_L(3) << 16);
}
