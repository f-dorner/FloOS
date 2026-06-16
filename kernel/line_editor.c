#include "line_editor.h"
#include "terminal.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_MAX_LENGTH 78

char input_buffer[INPUT_BUFFER_SIZE];
int input_length = 0;
int input_cursor = 0;

const char* line_editor_text(void)
{
    return input_buffer;
}

int line_editor_length(void)
{
    return input_length;
}

void line_editor_init(void)
{
    input_length = 0;
    input_cursor = 0;
    input_buffer[0] = '\0';
}

void line_editor_redraw_input(void)
{
    size_t row = terminal_get_row();

    terminal_clear_row_from(row, 1);

    for (int i = 0; i < input_length; i++) {
        terminal_put_char_at(input_buffer[i], i + 1, row);
    }

    terminal_set_cursor(input_cursor + 1, row);
}

void line_editor_backspace(void)
{
    if (input_cursor > 0) {
        for (int i = input_cursor - 1; i < input_length; i++) {
            input_buffer[i] = input_buffer[i + 1];
        }

        input_length--;
        input_cursor--;
        line_editor_redraw_input();
    }
}

void line_editor_insert(char c)
{
    if (input_length < INPUT_BUFFER_SIZE - 1 && input_length < INPUT_MAX_LENGTH) {
        for (int i = input_length; i > input_cursor; i--) {
            input_buffer[i] = input_buffer[i - 1];
        }

        input_buffer[input_cursor] = c;
        input_length++;
        input_cursor++;
        input_buffer[input_length] = '\0';

        line_editor_redraw_input();
    }
}

void line_editor_cursor_left(void)
{
    if (input_cursor > 0) {
        input_cursor--;
        line_editor_redraw_input();
    }
}

void line_editor_cursor_right(void)
{
    if (input_cursor < input_length) {
        input_cursor++;
        line_editor_redraw_input();
    }
}