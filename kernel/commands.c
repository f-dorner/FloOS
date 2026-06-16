#include "commands.h"
#include "terminal.h"
#include "string.h"
#include "fs.h"
#include "panic.h"
#include "reboot.h"
#include "system_info.h"
#include "parser.h"
#include "disk.h"
#include "shell.h"
#include "option.h"

#include <stddef.h>
#include <stdbool.h>

static char cmd_hex_digit(uint8_t value)
{
    //turns 0-15 into one hex character
    if (value < 10) {
        return '0' + value;
    }

    return 'A' + (value - 10);
}

static void cmd_cat_print_hex(const char* content, uint32_t size) 
{
    //print every character byte by byte as hex
    if (size == 0) {
        //empty
    } else {
        for (uint32_t i = 0; i < size; i++) {
            if (i > 0) {
                if ((i % 16) == 0) {
                    terminal_putchar('\n');
                } else {
                    terminal_putchar(' ');
                }
            }

            uint8_t byte = (uint8_t) content[i];
            uint8_t high = (byte >> 4) & 0x0F;
            uint8_t low = byte & 0x0F;

            terminal_putchar(cmd_hex_digit(high));
            terminal_putchar(cmd_hex_digit(low));
        }

        terminal_putchar('\n');
        return;
    }
}

static void cmd_cat_print_content(const char* content, uint32_t size)
{
    int col = 0;
    for (uint32_t i = 0; i < size; i++) {
        terminal_putchar(content[i]);
        col++;
        if (col >= 71) {
            terminal_set_cursor(0, terminal_get_row() + 1);
            col = 0;
        }
    }
    terminal_putchar('\n');
}

void cmd_help(const char* input)
{
    char name[32];
    char cmd_options[1][32];

    bool options = false;

    cmd_options[0][0] = '\0';

    parser_extract_options_no_args(input, name, cmd_options, 1);

    if (option_is_valid("help", cmd_options[0]) == 0) {
        if (strcmp(cmd_options[0], "options") == 0) {
            options = true;
        }
    }

    system_print_command_list(options);
}

void cmd_clear(void)
{
    terminal_clear();
}

void cmd_version(void) 
{
    terminal_writestring("FloOS v0.1\n");
}

void cmd_reboot(void)
{
    kernel_reboot();
}

void cmd_panic(void)
{
    kernel_panic();
}

void cmd_echo(const char* args)
{
    terminal_writestring(args);
    terminal_putchar('\n');
}

void cmd_ls(const char* input)
{
    char cmd_name[32];
    char options[2][32];

    bool all = false;
    bool long_format = false;

    options[0][0] = '\0';
    options[1][0] = '\0';

    //ls has no normal args, only options like --all or --long
    parser_extract_options_no_args(input, cmd_name, options, 2);

    if (option_is_valid("ls", options[0]) == 0) {
        if (strcmp(options[0], "all") == 0) {
            all = true;
        } else if (strcmp(options[0], "long") == 0) {
            long_format = true;
        }
    } 
    
    if (option_is_valid("ls", options[1]) == 0) {
        if (strcmp(options[1], "all") == 0) {
            all = true;
        } else if (strcmp(options[1], "long") == 0) {
            long_format = true;
        }
    }

    if (input[2] == '\0') {
        all = false;
        long_format = false;
    } else if (input[2] == ' ' && input[3] == '-' && input[4] == '-') {
        if (input[5] == '\0') {
            terminal_writestring("No option specified.\n");
            return;
        } else if (input[5] != '\0' && all == false && long_format == false) {
            terminal_writestring("Unknown option.\n");
            return;
        } 
    } else {
        terminal_writestring("Unknown command. Type 'help' for a list of commands.\n");
        return;
    }

    for (int i = 0; i < FS_MAX_FILES; i++) {
        FileEntry entry;

        //fs.c only gives the file slot, the commands.c decides what gets printed
        if (fs_get_entry(i, &entry) != 0) {
            continue;
        }

        if (entry.used == 1) {
            terminal_writestring(entry.name);
            terminal_putchar('\n');

            if (long_format == true) {
                terminal_writestring("start-sector: ");
                terminal_write_uint32_t(entry.start_sector);
                terminal_putchar('\n');
                terminal_writestring("size: ");
                terminal_write_uint32_t(entry.size);
                terminal_putchar('\n');
                terminal_writestring("used: ");
                terminal_write_uint8_t(entry.used);
                terminal_putchar('\n');
                terminal_writestring("sector-count: ");
                terminal_write_uint32_t(entry.sector_count);
                terminal_putchar('\n');
            }
        } else {
            if (all == true) {
                terminal_writestring("EMPTY SLOT\n");
            }
        }
    }
}

void cmd_touch(const char* args)
{   
    int result = fs_create(args);
    
    if (result == 0) {
        terminal_writestring("File created successfully.\n");
    } else if (result == 1) {
        terminal_writestring("File already exists!\n"); 
    } else if (result == 2) {
        terminal_writestring("File maximum reached!\n");
    }
}

void cmd_write(const char* input)
{
    char options[1][32];
    char name[32];

    options[0][0] = '\0';

    parser_extract_options_one_arg(input, name, options, 1);

    if (option_is_valid("write", options[0]) == 0) {
        if (strcmp(options[0], "clear") == 0) {
            int result = fs_clear(name);
            if (result == 0) {
                terminal_writestring("File content cleared successfully.\n");
            } else {
                terminal_writestring("File not found!\n");
            }
        }
    } else {
        int pos = 6; //after "write "
        while (input[pos] != ' ' && input[pos] != '\0') {
            pos++;
        }

        if (input[pos] == ' ' && input[pos + 1] != '-' && input[pos + 2] != '-') {
            terminal_writestring("Invalid syntax!\n");
        } else if (input[pos] == ' ' && input[pos + 1] == '-' && input[pos + 2] == '-') {
            terminal_writestring("Unknown option!\n");
        } else {
            shell_enter_editor(name);
        }
    }
}

void cmd_cat(const char* input)
{
    bool show_size = false;
    bool hex = false;

    char options[2][32];
    char name[32];

    options[0][0] = '\0';
    options[1][0] = '\0';

    //cat needs a filename and can have options after that
    parser_extract_options_one_arg(input, name, options, 2);

    int pos = 4;
    while (input[pos] != ' ' && input[pos] != '\0') {
        pos++;
    }

    if (input[pos] == ' ') {
        if (input[pos + 1] == '-' && input[pos + 2] == '-') {
            if ((options[0][0] != '\0' && option_is_valid("cat", options[0]) != 0) ||
                (options[1][0] != '\0' && option_is_valid("cat", options[1]) != 0)) {
                terminal_writestring("Unknown option!\n");
                return;
            }
        } else {
            terminal_writestring("Invalid syntax!\n");
            return;
        }
    }

    if (option_is_valid("cat", options[0]) == 0) {
        if (strcmp(options[0], "size") == 0) {
            show_size = true;
        } else if (strcmp(options[0], "hex") == 0) {
            hex = true;
        }
    } 
    
    if (option_is_valid("cat", options[1]) == 0) {
        if (strcmp(options[1], "size") == 0) {
            show_size = true;
        } else if (strcmp(options[1], "hex") == 0) {
            hex = true;
        }
    }

    char content[FS_SECTOR_SIZE * FS_SECTORS_PER_FILE];
    uint32_t size = 0;

    int result = fs_read(name, content, &size);
    
    if (result == 0) {
        if (show_size == true) {
            terminal_writestring("File size: ");
            terminal_write_uint32_t(size);
            terminal_writestring(" bytes.");
            terminal_putchar('\n');
        }

        if (hex == true) {
            cmd_cat_print_hex(content, size);
            return;
        }

        if (size == 0) {
            //empty
        } else {
            cmd_cat_print_content(content, size);
        }
    } else {
        terminal_writestring("File not found!\n");
    }
}

void cmd_rm(const char* input)
{
    bool force = false;

    char name[32];
    char options[1][32];

    options[0][0] = '\0';

    parser_extract_options_one_arg(input, name, options, 1);

    int pos = 3;
    while (input[pos] != ' ' && input[pos] != '\0') {
        pos++;
    }

    if (input[pos] == ' ') {
        if (input[pos + 1] == '-' && input[pos + 2] == '-') {
            if (options[0][0] != '\0' && option_is_valid("rm", options[0]) != 0) {
                terminal_writestring("Unknown option!\n");
                return;
            }
        } else {
            terminal_writestring("Invalid syntax!\n");
            return;
        }
    }

    if (option_is_valid("rm", options[0]) == 0) {
        if (strcmp(options[0], "force") == 0) {
            force = true;
        }
    }
    
    int result = fs_delete(name, force);
    
    if (result == 0) { 
        terminal_writestring("File deleted successfully.\n"); 
    } else if (result == 1) {
        terminal_writestring("File not found!\n");
    } else if (result == 2) {
        //empty
    }
}

void cmd_exec(const char* input)
{
    if (strcmp(input, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(input, "version") == 0) {
        cmd_version();
    } else if (strcmp(input, "reboot") == 0) {
        cmd_reboot();
    } else if (strcmp(input, "panic") == 0) {
        cmd_panic();
    } else if (strcmp(input, "ls") == 0 || parser_starts_with(input, "ls")) {
        cmd_ls(input);
    } else if (parser_starts_with(input, "echo ")) {
        cmd_echo(input + 5);
    } else if (parser_starts_with(input, "touch ")) {
        cmd_touch(input + 6);
    } else if (parser_starts_with(input, "write ")) {
        cmd_write(input);
    } else if (parser_starts_with(input, "cat ")) {
        cmd_cat(input);
    } else if (parser_starts_with(input, "rm ")) {
        cmd_rm(input);
    } else if (strcmp(input, "help") == 0 || parser_starts_with(input, "help ")) {
        cmd_help(input);
    } else {
        terminal_writestring("Unknown command. Type 'help' for a list of commands.\n");
    }
}
