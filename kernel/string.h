#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char* a, const char* b);
size_t strlen(const char* str);
void strcpy(char* dest, const char* src);

#endif