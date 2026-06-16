#include "string.h"

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }

    return len;
}

int strcmp(
    const char* a,
    const char* b
)
{
    int i = 0;

    while (a[i] && b[i]) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }

        i++;
    }

    return a[i] - b[i];
}

void strcpy(
    char* dest,
    const char* src
)
{
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}
