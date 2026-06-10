#include <stdbool.h>
#include "keyboard.h"
#include "io.h"
#include "terminal.h"
#include "shell.h"
#include "keymap_de.h"

uint8_t status;
uint8_t scancode;

bool shift_pressed = false;
bool ctrl_pressed = false;
bool alt_pressed = false;
bool capslock_enabled = false;

void keyboard_poll(void)
{
    status = inb(0x64);

    if (status & 1) {
        keyboard_read_scancode();
    }
}

void keyboard_read_scancode(void) 
{
    scancode = inb(0x60);

    switch (scancode)
    {
        // LEFT SHIFT
        case 0x2A:
            shift_pressed = true;
            return;

        case 0xAA:
            shift_pressed = false;
            return;

        // RIGHT SHIFT
        case 0x36:
            shift_pressed = true;
            return;

        case 0xB6:
            shift_pressed = false;
            return;

        // CTRL
        case 0x1D:
            ctrl_pressed = true;
            return;

        case 0x9D:
            ctrl_pressed = false;
            return;

        // ALT
        case 0x38:
            alt_pressed = true;
            return;

        case 0xB8:
            alt_pressed = false;
            return;

        // CAPS LOCK
        case 0x3A:
            capslock_enabled = !capslock_enabled;
            return;
        
        case 0x4B:
            shell_cursor_left();
            return;

        case 0x4D:
            shell_cursor_right();
            return;
    }

    if (scancode >= 128) {
        return;
    }

    char text[8];

    size_t count = keymap_de_translate(
        scancode,
        shift_pressed,
        alt_pressed,
        text,
        sizeof(text)
    );

    for(size_t i = 0; i < count; i++) {
        shell_handle_input(text[i]);
    }
}
