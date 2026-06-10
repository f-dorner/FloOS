#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_read_scancode(void);
void keyboard_poll(void);

#endif