#include "shell.h"
#include "commands.h"
#include "terminal.h"
#include "line_editor.h"
#include "editor.h"

static ShellMode shell_mode = SHELL_MODE_COMMAND;

void shell_init(void)
{
    line_editor_init();
}

void shell_prompt(void)
{
    terminal_putchar('>');
}

void shell_handle_input(char c)
{
    if (shell_mode == SHELL_MODE_EDITOR) {
        if (editor_handle_input(c) == true) {
            shell_mode = SHELL_MODE_COMMAND;
            shell_prompt();
        }

        return;
    }

    if (c == '\n') {
        if (line_editor_length() == 0) {
            return;
        }

        terminal_putchar('\n');
        shell_execute(line_editor_text());
        line_editor_init();

        if (shell_mode == SHELL_MODE_COMMAND) {
            shell_prompt();
        }

        return;
    } else if (c == '\b') {
        line_editor_backspace();
        return;
    }

    line_editor_insert(c);
}

void shell_cursor_left(void)
{
    if (shell_mode == SHELL_MODE_EDITOR) {
        editor_cursor_left();
    } else {
        line_editor_cursor_left();
    }
}

void shell_cursor_right(void)
{
    if (shell_mode == SHELL_MODE_EDITOR) {
        editor_cursor_right();
    } else {
        line_editor_cursor_right();
    }
}

void shell_execute(const char* input)
{
    cmd_exec(input);
}

void shell_enter_editor(const char* filename)
{
    shell_mode = SHELL_MODE_EDITOR;
    editor_open(filename);
}