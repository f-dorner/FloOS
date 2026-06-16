#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>

void editor_open(const char* filename);
bool editor_handle_input(char c);
void editor_cursor_left(void);
void editor_cursor_right(void);

#endif