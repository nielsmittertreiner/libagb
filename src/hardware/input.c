#include "agb.h"

#define KEYS_MASK           0x03FF
#define KEYS_MASK_EXCL_DPAD 0x030F

typedef struct
{
    uint16_t new;
    uint16_t held;
    uint16_t repeated;
    uint16_t repeat_counter;
    uint16_t repeat_start_delay;
    uint8_t repeat_continue_delay;
} key_input;

IWRAM_DATA static key_input s_input;

void input_init(uint8_t repeat_start_delay, uint8_t repeat_continue_delay)
{
    s_input.repeat_counter = repeat_start_delay;
    s_input.repeat_start_delay = repeat_start_delay;
    s_input.repeat_continue_delay = repeat_continue_delay;
}

void input_read(void)
{
    uint16_t input = REG_KEYINPUT ^ KEYS_MASK;

    s_input.new = input & ~s_input.held;
    s_input.repeated = s_input.new;

    if (input != 0 && s_input.held == input)
    {
        s_input.repeat_counter--;

        if (s_input.repeat_counter == 0)
        {
            s_input.repeated = input;
            s_input.repeat_counter = s_input.repeat_continue_delay;
        }
    }
    else
    {
        s_input.repeat_counter = s_input.repeat_start_delay;
    }

    s_input.held = input;
}

uint8_t input_new(keys keys)
{
    if (s_input.new & keys)
        return TRUE;
    
    return FALSE;
}

uint8_t input_held(keys keys)
{
    if (s_input.held & keys)
        return TRUE;
    
    return FALSE;
}

uint8_t input_repeated(keys keys)
{
    if (s_input.repeated & keys)
        return TRUE;
    
    return FALSE;
}
