#include "terminal.h"
#include "vga.h"
#include "io.h"
#include "string.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 1;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    terminal_clear();
}

void terminal_clear(void) 
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

void terminal_clear_row_from(size_t row, size_t start_col)
{
    for (size_t col = start_col; col < 80; col++) {
        terminal_putentryat(' ', terminal_color, col, row);
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_put_char_at(char c, size_t x, size_t y)
{
    terminal_putentryat(c, terminal_color, x, y);
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
        return;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 1;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }

    update_cursor(terminal_column, terminal_row);
}

void terminal_scroll(void) 
{
    for (size_t i = VGA_WIDTH; i < (VGA_HEIGHT * VGA_WIDTH); i++)  {   
        terminal_buffer[(i - VGA_WIDTH)] = terminal_buffer[i];
    } 

    for (size_t i = ((VGA_HEIGHT - 1) * VGA_WIDTH); i < (VGA_HEIGHT * VGA_WIDTH); i++)  {
        terminal_buffer[i] = vga_entry(' ', terminal_color);
    }

    terminal_row = VGA_HEIGHT - 1;
}

void terminal_write(const char* data, size_t size) 
{
    int word_length = 0;
    int j = 0;

    for (size_t i = 0; i < size; i++) {
        if (data[i] != ' ' && data[i] != '\n' && data[i] != '\0') {
            j = i;
            while (data[j] != ' ' && data[j] != '\n' && data[j] != '\0') {
                j++;
            }

            word_length = j - i;

            if (terminal_column + word_length > VGA_WIDTH) {
                terminal_column = 0;
                terminal_row++;
                if (terminal_row == VGA_HEIGHT) {
                    terminal_scroll();
                }
                word_length = 0;
            } 

        } else {
            word_length = 0;
        }

        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
    update_cursor(terminal_column, terminal_row);
}

void terminal_write_uint8_t(uint8_t data)
{
    int len = uint8_t_len(data);
    char str[len + 1];

    str[len] = '\0';

    for (int i = len - 1; i >= 0; i--) {
        int digit = data % 10;
        str[i] = '0' + digit;
        data = data / 10;
    }

    terminal_writestring(str);
}

void terminal_write_uint32_t(uint32_t data)
{
    int len = uint32_t_len(data);
    char str[len + 1];

    str[len] = '\0';

    for (int i = len - 1; i >= 0; i--) {
        int digit = data % 10;
        str[i] = '0' + digit;
        data = data / 10;
    }

    terminal_writestring(str);
}

int uint8_t_len(uint8_t value)
{
    int len = 1;

    while (value >= 10) {
        value = value / 10;
        len++;
    }
    
    return len;
}

int uint32_t_len(uint32_t value)
{
    int len = 1;

    while (value >= 10) {
        value = value / 10;
        len++;
    }
    
    return len;
}

void terminal_set_cursor(size_t x, size_t y)
{
    terminal_column = x;
    terminal_row = y;
    update_cursor(terminal_column, terminal_row);
}

void update_cursor(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

size_t terminal_get_row(void)
{
    return terminal_row;
}

size_t terminal_get_col(void) {
    return terminal_column;
}

