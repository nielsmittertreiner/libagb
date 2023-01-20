#include "agb.h"

static uint8_t irq_seach_index(irq_flag irq);
extern void irq_main(void);

IWRAM_DATA irq_vector irq_vector_table[IRQ_COUNT];

void irq_init(void)
{
    REG_IME	= 0;

    for (uint32_t i = 0; i < IRQ_COUNT; i++)
        irq_vector_table[i] = NULL;
    
    IRQ_VECTOR = irq_main;
    REG_IE = 0;
    REG_IF = 0x3FFF;
    REG_IME = 1;
}

void irq_set(irq_flag irq, irq_vector func)
{
    uint8_t i = irq_seach_index(irq);
    irq_vector_table[i] = func;
}

void irq_enable(irq_flag mask)
{
    REG_IME	= 0;

    if (mask & IRQ_FLAG_VBLANK)
    {
        REG_DISPSTAT |= DISPSTAT_VBLANK_INTR;
    }

	if (mask & IRQ_FLAG_HBLANK)
    {
        REG_DISPSTAT |= DISPSTAT_HBLANK_INTR;
    }

	if (mask & IRQ_FLAG_VCOUNT)
    {
        REG_DISPSTAT |= DISPSTAT_VCOUNT_INTR;
    }

    REG_IE |= mask;
    REG_IME	= 1;
}

void irq_disable(irq_flag mask)
{
    REG_IME	= 0;

	if (mask & IRQ_FLAG_VBLANK)
    {
        REG_DISPSTAT &= ~DISPSTAT_VBLANK_INTR;
    }
    
	if (mask & IRQ_FLAG_HBLANK)
    {
        REG_DISPSTAT &= ~DISPSTAT_HBLANK_INTR;
    }
    
	if (mask & IRQ_FLAG_VCOUNT)
    {
        REG_DISPSTAT &= ~DISPSTAT_VCOUNT_INTR;
    }
    
	REG_IE &= ~mask;
	REG_IME	= 1;
}

void irq_set_vcount(uint8_t scanline)
{
    REG_DISPSTAT |= (scanline << 8);
}

static uint8_t irq_seach_index(irq_flag irq)
{
    uint8_t i = 0;

    while (irq != 1)
    {
        irq >>= 1;
        i++;
    }
    return i;
}
