#ifndef GUARD_AGB_HARDWARE_INPUT_H
#define GUARD_AGB_HARDWARE_INPUT_H

#include "agb.h"

typedef enum
{
    A_BUTTON            = BIT(0),
    B_BUTTON            = BIT(1),
    SELECT_BUTTON       = BIT(2),
    START_BUTTON        = BIT(3),
    DPAD_RIGHT          = BIT(4),
    DPAD_LEFT           = BIT(5),
    DPAD_UP             = BIT(6),
    DPAD_DOWN           = BIT(7),
    R_BUTTON            = BIT(8),
    L_BUTTON            = BIT(9),
    DPAD_ANY            = (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN),
    SHOULDER_BUTTONS    = (R_BUTTON | L_BUTTON),
    ACTION_BUTTONS      = (A_BUTTON | B_BUTTON),
    START_SELECT        = (SELECT_BUTTON | START_BUTTON),
    KEYS_ANY            = (BITS(10)),
} keys;

typedef enum
{
    KEY_INTR_ENABLE = BIT(14),
    KEY_INTR_AND    = BIT(15),
    KEY_INTR_OR     = !KEY_INTR_AND,
} key_interrupt;

void input_init(uint8_t repeat_start_delay, uint8_t repeat_continue_delay);
void input_read(void);
uint8_t input_new(keys keys);
uint8_t input_held(keys keys);
uint8_t input_repeated(keys keys);

#endif // GUARD_AGB_HARDWARE_INPUT_H