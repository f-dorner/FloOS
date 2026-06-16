#ifndef SHELL_H
#define SHELL_H

typedef enum {
    SHELL_MODE_COMMAND,
    SHELL_MODE_EDITOR
} ShellMode;

void shell_init(void);
void shell_prompt(void);
void shell_handle_input(char c);
void shell_cursor_left(void);
void shell_cursor_right(void);
void shell_execute(const char* input);
void shell_enter_editor(const char* filename);

#endif