
#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

void shell_init(void);
void shell_prompt(void);
void shell_handle_input(char c);
void shell_cursor_left(void);
void shell_cursor_right(void);
void shell_execute(const char* input);

#endif
