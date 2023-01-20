#include "agb.h"

#define KEYS_MASK           0x03FF
#define KEYS_MASK_EXCL_DPAD 0x030F

typedef struct key_input
{
    uint16_t new;
    uint16_t held;
    uint16_t repeated;
    uint16_t counter;
    uint16_t repeat_start_delay;
    uint16_t repeat_continue_delay;
} key_input;

EWRAM_DATA ALIGNED(4) static key_input input;

void input_init(uint16_t repeat_start_delay, uint16_t repeat_continue_delay)
{
    input.counter = repeat_start_delay;
    input.repeat_start_delay = repeat_start_delay;
    input.repeat_continue_delay = repeat_continue_delay;
}

void input_read(void)
{
    uint16_t current = ~REG_KEYINPUT & KEYS_MASK;

    input.new = current & ~input.held;
    input.repeated = input.new;

    if (current && current == input.held)
    {
        input.counter--;

        if (input.counter == 0)
        {
            input.repeated = current;
            input.counter = input.repeat_continue_delay;
        }
    }
    else
    {
        input.counter = input.repeat_start_delay;
    }

    input.held = current;
}

inline uint8_t input_new(keys keys)
{
    return input.new & keys;
}

inline uint8_t input_held(keys keys)
{
    return input.held & keys;
}

inline uint8_t input_repeated(keys keys)
{
    return input.repeated & keys;
}

inline vec2i_t input_dpad_direction(void)
{
    return (vec2i_t){((input.held >> 4) & 1) - ((input.held >> 5) & 1), ((input.held >> 7) & 1) - ((input.held >> 6) & 1)};
}
