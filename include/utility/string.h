#ifndef GUARD_AGB_UTILITY_STRING_H
#define GUARD_AGB_UTILITY_STRING_H

#include "agb.h"

uint32_t strlen(const char *str);
char *strcpy(const char *restrict src, char *restrict dst);
char *strcat(const char *restrict src, char *restrict dst);
int32_t strcmp(const char *restrict str1, const char *restrict str2);

#endif // GUARD_AGB_UTILITY_STRING_H
