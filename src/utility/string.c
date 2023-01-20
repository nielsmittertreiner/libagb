#include "agb.h"

uint32_t strlen(const char *str)
{
    uint32_t len = 0;

    while (str[len] != '\0')
        len++;
    return len;
}

char *strcpy(const char *src, char *dst)
{
    while ((*dst++ = *src++) != '\0');
    return dst;
}

char *strcat(const char *src, char *dst)
{
    while (*dst++ != '\0');
    return strcpy(src, dst);
}

int32_t strcmp(const char *str1, const char *str2)
{
    while(*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return *(const uint8_t *)str1 - *(const uint8_t *)str2;
}
