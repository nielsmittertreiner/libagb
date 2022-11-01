#ifndef GUARD_AGB_HARDWARE_INTERRUPTS_H
#define GUARD_AGB_HARDWARE_INTERRUPTS_H

#include "agb.h"

#define IRQ_COUNT 14

typedef void (*irq_vector)(void);

typedef enum
{
    IRQ_FLAG_VBLANK     = BIT(0),
    IRQ_FLAG_HBLANK     = BIT(1),
    IRQ_FLAG_VCOUNT     = BIT(2),
    IRQ_FLAG_TIMER_0    = BIT(3),
    IRQ_FLAG_TIMER_1    = BIT(4),
    IRQ_FLAG_TIMER_2    = BIT(5),
    IRQ_FLAG_TIMER_3    = BIT(6),
    IRQ_FLAG_SERIAL     = BIT(7),
    IRQ_FLAG_DMA_0      = BIT(8),
    IRQ_FLAG_DMA_1      = BIT(9),
    IRQ_FLAG_DMA_2      = BIT(10),
    IRQ_FLAG_DMA_3      = BIT(11),
    IRQ_FLAG_KEYPAD     = BIT(12),
    IRQ_FLAG_GAMEPAK    = BIT(13),
} irq_flag;

extern irq_vector s_irq_vector_table[IRQ_COUNT];

void irq_init(void);
void irq_set(irq_flag irq, irq_vector func);
void irq_enable(irq_flag mask);
void irq_disable(irq_flag mask);
void irq_set_vcount(uint8_t scanline);
void irq_main(void);

#endif // GUARD_AGB_HARDWARE_INTERRUPTS_H