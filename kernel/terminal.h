#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_clear(void);
void terminal_clear_row_from(size_t row, size_t start_col);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_put_char_at(char c, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_scroll(void);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_write_uint8_t(uint8_t data);
void terminal_write_uint32_t(uint32_t data);
int uint8_t_len(uint8_t value);
int uint32_t_len(uint32_t value);
void terminal_set_cursor(size_t x, size_t y);
void update_cursor(int x, int y);
size_t terminal_get_row(void);

#endif
