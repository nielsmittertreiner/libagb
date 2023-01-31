#include <stdarg.h>
#include "agb.h"

#define REG_MGBA_DEBUG_ENABLE (*(volatile uint16_t *)0x4FFF780)
#define REG_MGBA_DEBUG_FLAGS  (*(volatile uint16_t *)0x4FFF700)
#define REG_MGBA_DEBUG_STRING ((char*)0x4FFF600)

#define REG_NOCASHGBA_ENABLE  ((uint8_t *)0x04FFFA00)
#define REG_NOCASHGBA_STRING  (*(volatile uint32_t *)0x04FFFA14)


typedef enum log_interface
{
    LOG_NONE,
    LOG_MGBA,
    LOG_NOCASHGBA,
} log_interface;

typedef struct log_buffer
{
    char *buffer;
    char *pbuffer;
    uint32_t size;
} log_buffer;

typedef struct log_interface_info
{
    uint32_t id;
    uint32_t (*init)(void);
    int32_t (*printf)(log_level, const char *, va_list);
    const char *name;
} log_interface_info;

static uint32_t init_print_none(void);
static int32_t printf_none(log_level level, const char *str, va_list args);
static uint32_t init_print_mgba(void);
static int32_t printf_mgba(log_level level, const char *str, va_list args);
static uint32_t init_print_nocashgba(void);
static int32_t printf_nocashgba(log_level level, const char *str, va_list args);

static int32_t agb_vsnprintf(char *buffer, const char *src, size_t size, va_list args);
static int32_t agb_vpprintf(void *buffer, const char *src, va_list args);
static int32_t agb_puts(char *str, uint32_t len, void *buffer);
static int32_t agb_itoa(int32_t val, uint32_t radix, int32_t uppercase, uint32_t unsig, char *buffer);
static int32_t agb_pad(char *ptr, uint32_t len, char pad_char, int32_t pad_to, char *buffer);

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

static const log_interface s_log_interface_search_order[] =
{
    LOG_MGBA,
    LOG_NOCASHGBA,
    LOG_NONE,
};

static EWRAM_DATA const log_interface_info *s_active_log_interface = &s_log_interface_info[LOG_NONE];

uint32_t agb_print_init(void)
{
    const log_interface *search = s_log_interface_search_order;
    const log_interface_info *logger;

    while ((logger = &s_log_interface_info[*search]), !logger->init())
        search++;

    s_active_log_interface = logger;

    return logger->id;
}

int32_t agb_printf(log_level level, const char *str, ...)
{
    va_list args;
    int32_t n;

    va_start(args, str);
    n = s_active_log_interface->printf(level, str, args);
    va_end(args);
    return n;
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

static int32_t printf_none(log_level level, const char *str, va_list args)
{
    (void)level;
    (void)str;
    (void)args;
    return 0;
}

static uint32_t init_print_mgba(void)
{
    REG_MGBA_DEBUG_ENABLE = 0xC0DE;
	return REG_MGBA_DEBUG_ENABLE == 0x1DEA;
}

static int32_t printf_mgba(log_level level, const char *str, va_list args)
{
    int32_t n;

	level &= 0x7;
	n = agb_vsnprintf(REG_MGBA_DEBUG_STRING, str, 0x100, args);
	REG_MGBA_DEBUG_FLAGS = level | 0x100;
    return n;
}

static uint32_t init_print_nocashgba(void)
{
    for (uint32_t i = 0; i < 6; i++)
    {
        if ("no$gba"[i] != REG_NOCASHGBA_ENABLE[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

static int32_t printf_nocashgba(log_level level, const char *str, va_list args)
{
    int32_t n;
    const char newline[2] = "\n";
    char buffer[0x100];
    (void)level;
    
    n = agb_vsnprintf(buffer, str, 0x100, args);
    strcat(newline, buffer);
    REG_NOCASHGBA_STRING = (uint32_t)buffer;
    return n;
}

static int32_t agb_vsnprintf(char *buffer, const char *src, size_t size, va_list args)
{
    log_buffer b;
    
    b.buffer = buffer;
    b.pbuffer = buffer;
    b.size = size;

    agb_vpprintf(&b, src, args);
    return b.pbuffer - b.buffer;
}

static int32_t agb_vpprintf(void* buffer, const char *src, va_list args)
{
    char bf[24];
    char bf2[24];
    char ch;
    int32_t n = 0;

    while ((ch = *(src++)))
    {
        int32_t len;

        if (ch != '%')
        {
            len = 1;
            len = agb_puts(&ch, len, buffer);
        }
        else
        {
            char pad_char = ' ';
            int32_t pad_to = 0;
            char l = 0;
            char *ptr;

            ch = *(src++);

            if (ch == '0')
            {
                pad_char = '0';
            }

            while (ch >= '0' && ch <= '9')
            {
                pad_to = pad_to * 10 + (ch - '0');
                ch = *(src++);
            }

            if(pad_to > (int32_t) sizeof(bf))
            {
                pad_to = sizeof(bf);
            }

            if (ch == 'l')
            {
                l = 1;
                ch = *(src++);
            }

            switch (ch)
            {
                case 0:
                    goto end;
                case 'u':
                case 'd':
                    if (l)
                    {
                        len = agb_itoa(va_arg(args, uint32_t), 10, 0, (ch=='u'), bf2);
                    }
                    else
                    {
                        if(ch == 'u')
                        {
                            len = agb_itoa((uint32_t) va_arg(args, uint32_t), 10, 0, 1, bf2);
                        }
                        else
                        {
                            len = agb_itoa((int32_t) va_arg(args, int32_t), 10, 0, 0, bf2);
                        }
                    }
                    len = agb_pad(bf2, len, pad_char, pad_to, bf);
                    len = agb_puts(bf, len, buffer);
                    break;
                case 'x':
                case 'X':
                    if(l)
                    {
                        len = agb_itoa(va_arg(args, uint32_t), 16, (ch=='X'), 1, bf2);
                    }
                    else
                    {
                        len = agb_itoa((uint32_t) va_arg(args, uint32_t), 16, (ch=='X'), 1, bf2);
                    }
                    len = agb_pad(bf2, len, pad_char, pad_to, bf);
                    len = agb_puts(bf, len, buffer);
                    break;
                case 'c' :
                    ch = (char)(va_arg(args, int32_t));
                    len = agb_pad(&ch, 1, pad_char, pad_to, bf);
                    len = agb_puts(bf, len, buffer);
                    break;
                case 's' :
                    ptr = va_arg(args, char*);
                    len = strlen(ptr);
                    if (pad_to > 0)
                    {
                        len = agb_pad(ptr, len, pad_char, pad_to, bf);
                        len = agb_puts(bf, len, buffer);
                    } else
                    {
                        len = agb_puts(ptr, len, buffer);
                    }
                    break;
                default:
                    len = 1;
                    len = agb_puts(&ch, len, buffer);
                    break;
            }
        }
        n += len;
    }
end:
    return n;
}

static int32_t agb_puts(char *str, uint32_t len, void *buffer)
{
    char *p0;
    log_buffer *b = buffer;
    
    p0 = b->buffer;
    for (uint32_t i = 0; i < len; i++)
    {
        if (b->pbuffer == b->buffer + b->size - 1)
            break;

        *(b->pbuffer++) = str[i];
    }

    *(b->pbuffer) = 0;
    return (int32_t)(b->pbuffer - p0);
}

static int32_t agb_itoa(int32_t val, uint32_t radix, int32_t uppercase, uint32_t unsig, char *buffer)
{
    char *pbuffer = buffer;
    int32_t negative = 0;
    int32_t i, len;

    /* No support for unusual radixes. */
    if (radix > 16)
        return 0;

    if (val < 0 && !unsig)
    {
        negative = 1;
        val = -val;
    }

    /* This builds the string back to front ... */
    do 
    {
        int32_t digit = val % radix;
        *(pbuffer++) = (digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10);
        val /= radix;
    } while (val > 0);

    if (negative)
        *(pbuffer++) = '-';

    *(pbuffer) = '\0';

    /* ... now we reverse it (could do it recursively but will
     * conserve the stack space) */
    len = (pbuffer - buffer);
    for (i = 0; i < len / 2; i++)
    {
        char j = buffer[i];
        buffer[i] = buffer[len-i-1];
        buffer[len-i-1] = j;
    }

    return len;
}

static int32_t agb_pad(char *ptr, uint32_t len, char pad_char, int32_t pad_to, char *buffer)
{
    uint32_t overflow = FALSE;
    char *pbuffer = buffer;

    if (pad_to == 0)
    {
        pad_to = len;
    }

    if (len > pad_to)
    {
        len = pad_to;
        overflow = TRUE;
    }

    for (uint32_t i = pad_to - len; i > 0; i--)
    {
        *(pbuffer++) = pad_char;
    }

    for (uint32_t i = len; i > 0; i--)
    {
        *(pbuffer++) = *(ptr++);
    }

    len = pbuffer - buffer;

    if (overflow)
    {
        for (uint32_t i = 0; i < 3 && pbuffer > buffer; i++)
        {
            *(pbuffer-- - 1) = '*';
        }
    }

    return len;
}
