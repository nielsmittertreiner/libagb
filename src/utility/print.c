#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "agb.h"

#define REG_MGBA_DEBUG_ENABLE (*(volatile uint16_t *)0x4FFF780)
#define REG_MGBA_DEBUG_FLAGS  (*(volatile uint16_t *)0x4FFF700)
#define REG_MGBA_DEBUG_STRING ((char*)0x4FFF600)

#define REG_NOCASHGBA_ENABLE  ((uint8_t *)0x04FFFA00)
#define REG_NOCASHGBA_STRING  (*(volatile uint32_t *)0x04FFFA14)

typedef enum
{
    LOG_NONE,
    LOG_MGBA,
    LOG_NOCASHGBA,
} log_interface;

typedef struct
{
    uint32_t id;
    uint32_t (*init)(void);
    void (*printf)(log_level, const char *, va_list);
    const char *name;
} log_interface_info;

static uint32_t init_print_none(void);
static void printf_none(log_level level, const char *str, va_list args);
static uint32_t init_print_mgba(void);
static void printf_mgba(log_level level, const char *str, va_list args);
static uint32_t init_print_nocashgba(void);
static void printf_nocashgba(log_level level, const char *str, va_list args);

static const log_interface_info s_log_interface_info[] =
{
    {
        .id = LOG_NONE,
        .init = init_print_none, 
        .printf = printf_none,
        .name = "None",
    },
    {
        .id = LOG_MGBA,
        .init = init_print_mgba, 
        .printf = printf_mgba,
        .name = "mGBA",
    },
    {
        .id = LOG_NOCASHGBA,
        .init = init_print_nocashgba, 
        .printf = printf_nocashgba,
        .name = "no$gba",
    },
};

static const log_interface_info *s_active_log_interface = &s_log_interface_info[LOG_NONE];

static const log_interface s_log_interface_search_order[] =
{
    LOG_MGBA,
    LOG_NOCASHGBA,
    LOG_NONE,
};

uint32_t agb_print_init(void)
{
    const log_interface *search = s_log_interface_search_order;
    const log_interface_info *logger;

    while ((logger = &s_log_interface_info[*search]), !logger->init())
        search++;

    s_active_log_interface = logger;

    return logger->id;
}

void agb_printf(log_level level, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    s_active_log_interface->printf(level, str, args);
    va_end(args);
}

void agb_assert(const char* condition, const char* error_message, const char* function, const char* file_name, uint32_t line)
{
    agb_printf(LOG_LEVEL_ERROR, "\nASSERTION FAILED: %s\nEXP: %s\nFUNC: %s\nFILE: %s\nLINE: %d", error_message, condition, function, file_name, line);
    asm(".hword 0xEFFF");
}

static uint32_t init_print_none(void)
{
    return TRUE;
}

static void printf_none(log_level level, const char *str, va_list args)
{
    (void)level;
    (void)str;
    (void)args;
}

static uint32_t init_print_mgba(void)
{
    REG_MGBA_DEBUG_ENABLE = 0xC0DE;
	return REG_MGBA_DEBUG_ENABLE == 0x1DEA;
}

static void printf_mgba(log_level level, const char *str, va_list args)
{
	level &= 0x7;
	vsnprintf(REG_MGBA_DEBUG_STRING, 0x100, str, args);
	REG_MGBA_DEBUG_FLAGS = level | 0x100;
}

static uint32_t init_print_nocashgba(void)
{
    for (int i = 0; i < 6; i++)
    {
        if ("no$gba"[i] != REG_NOCASHGBA_ENABLE[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

static void printf_nocashgba(log_level level, const char *str, va_list args)
{
    (void)level;
    const char newline[2] = "\n";
    char buffer[0x100];
    
    vsnprintf(buffer, 0x100, str, args);
    strcat(newline, buffer);
    REG_NOCASHGBA_STRING = (uint32_t)buffer;
}
