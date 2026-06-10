#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t keymap_de_translate(
    uint8_t scancode,
    bool shift,
    bool alt,
    char* output,
    size_t output_size
);

#endif
