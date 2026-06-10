#include "shell.h"
#include "commands.h"
#include "terminal.h"
#include "line_editor.h"

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
    if(c == '\n') 
    {
        if (line_editor_length() == 0) 
        {
            return;
        }

        terminal_putchar('\n');
        shell_execute(line_editor_text());
        line_editor_init();
        shell_prompt();
        return;

    } else if (c == '\b') 
    {
        line_editor_backspace();
        return;
    }

    line_editor_insert(c);
}

void shell_cursor_left(void)
{
    line_editor_cursor_left();
}

void shell_cursor_right(void)
{
    line_editor_cursor_right();
}

void shell_execute(const char* input)
{
    cmd_exec(input);
}
