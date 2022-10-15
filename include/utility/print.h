#ifndef GUARD_AGB_UTILITY_PRINT_H
#define GUARD_AGB_UTILITY_PRINT_H

#include "agb.h"

// Fix this stuff
// Crashes system on purpose, will print error info in console
#define AGB_ASSERT(condition, error_message)                                \
    if(!(condition))                                                        \
        agb_assert(#condition, #error_message, __func__, __FILE__, __LINE__);

typedef enum
{
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level;

uint32_t agb_print_init(void);
void agb_printf(log_level level, const char* str, ...);
void agb_assert(const char* condition, const char* error_message, const char* function, const char* file_name, uint32_t line);

#endif // GUARD_AGB_UTILITY_PRINT_H
