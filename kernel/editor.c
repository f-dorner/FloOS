#include "editor.h"
#include "terminal.h"
#include "fs.h"
#include "string.h"

#define EDITOR_START_COL 1
#define EDITOR_TEXT_WIDTH 71
#define EDITOR_LINE "------------------------------------------------------------------------"

static char editor_filename[32];
static char editor_buffer[FS_SECTOR_SIZE * FS_SECTORS_PER_FILE];
static int editor_length = 0;
static int editor_cursor = 0;
static size_t editor_row = 0;

static void editor_backspace(void);
static void editor_redraw(void);
static void editor_insert(char c);

void editor_open(const char* filename)
{
    strcpy(editor_filename, filename);

    terminal_writestring("------------------------------------------------------------------------\n");
    terminal_writestring("Write editor for: ");
    terminal_writestring(filename);
    terminal_putchar('\n');

    terminal_putchar('>');

    editor_row = terminal_get_row();

    uint32_t size = 0;

    if (fs_read(filename, editor_buffer, &size) == 0) {
        editor_length = size;
        editor_cursor = size;
        editor_buffer[editor_length] = '\0';
    } else {
        editor_length = 0;
        editor_cursor = 0;
        editor_buffer[0] = '\0';
    }

    editor_redraw();
}

bool editor_handle_input(char c)
{
    if (c == '\n') {
        fs_write(editor_filename, editor_buffer);

        editor_buffer[0] = '\0';
        editor_length = 0;
        editor_cursor = 0;

        terminal_writestring("\n\nEditor closed.\n");
        return true;
    }

    if (c == '\b') {
        editor_backspace();
        return false;
    }

    editor_insert(c);
    return false;
}

void editor_cursor_left(void)
{
    if (editor_cursor > 0) {
        editor_cursor--;
        editor_redraw();
    }
}

void editor_cursor_right(void)
{
    if (editor_cursor < editor_length) {
        editor_cursor++;
        editor_redraw();
    }
}

static void editor_backspace(void)
{
    if (editor_cursor > 0) {
        for (int i = editor_cursor - 1; i < editor_length; i++) {
            editor_buffer[i] = editor_buffer[i + 1];
        }

        editor_cursor--;
        editor_length--;
        editor_buffer[editor_length] = '\0';

        editor_redraw();
    }
}

static void editor_redraw(void)
{
    int used_rows = (editor_length / EDITOR_TEXT_WIDTH) + 1;

    for (int r = 0; r < used_rows; r++) {
        if (r == 0) {
            terminal_clear_row_from(editor_row + r, EDITOR_START_COL);
        } else {
            terminal_clear_row_from(editor_row + r, 0);
        }
    }

    for (int i = 0; i < editor_length; i++) {
        int col = EDITOR_START_COL + (i % EDITOR_TEXT_WIDTH);
        int row = editor_row + (i / EDITOR_TEXT_WIDTH);

        terminal_put_char_at(editor_buffer[i], col, row);
    }

    int bottom_row = editor_row + used_rows;
    terminal_set_cursor(0, bottom_row);
    terminal_writestring(EDITOR_LINE);

    int max_rows = ((FS_SECTOR_SIZE * FS_SECTORS_PER_FILE) + EDITOR_TEXT_WIDTH - 1) / EDITOR_TEXT_WIDTH;

    for (int r = bottom_row + 1; r < max_rows; r++) {
        terminal_clear_row_from(r, 0);
    }

    int cursor_col = EDITOR_START_COL + (editor_cursor % EDITOR_TEXT_WIDTH);
    int cursor_row = editor_row + (editor_cursor / EDITOR_TEXT_WIDTH);

    terminal_set_cursor(cursor_col, cursor_row);
}

static void editor_insert(char c)
{
    if (editor_length < (FS_SECTOR_SIZE * FS_SECTORS_PER_FILE) - 1) {
        for (int i = editor_length; i > editor_cursor; i--) {
            editor_buffer[i] = editor_buffer[i - 1];
        }

        editor_buffer[editor_cursor] = c;
        editor_length++;
        editor_cursor++;
        editor_buffer[editor_length] = '\0';

        editor_redraw();
    }
}