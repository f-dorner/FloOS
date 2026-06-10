#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <stddef.h>
#include <stdint.h>

int line_editor_length(void);
const char* line_editor_text(void);
void line_editor_init(void);
void line_editor_redraw_input(void);
void line_editor_backspace(void);
void line_editor_insert(char c);
void line_editor_cursor_left(void);
void line_editor_cursor_right(void);

#endif