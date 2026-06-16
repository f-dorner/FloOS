#include "system_info.h"
#include "terminal.h"
#include "option.h"

void system_print_boot_message(void) 
{
    terminal_writestring("FloOS kernel bootet.\n");
    terminal_writestring("Terminal initialized.\n");
    terminal_writestring("VGA text mode: 80x25\n");
    terminal_writestring("Type 'help' for a list of commands.\n");
}

void system_print_command_list(bool options)
{

    if (options == true) {
            terminal_writestring("List of commands with options:\n");
            terminal_writestring("------------------------------\n");

            terminal_writestring("help      ");
            for (int i = 0; i < 1; i++) {
                terminal_writestring("--");
                terminal_writestring(help_options[i]);
            }

            terminal_putchar('\n');

            terminal_writestring("rm        ");
            for (int i = 0; i < 1; i++) {
                terminal_writestring("--");
                terminal_writestring(rm_options[i]);
            }

            terminal_putchar('\n');

            terminal_writestring("ls        ");
            for (int i = 0; i < 2; i++) {
                terminal_writestring("--");
                terminal_writestring(ls_options[i]);

                if (i != 1) {
                    terminal_writestring(", ");
                }
            }           

            terminal_putchar('\n');

            terminal_writestring("write     ");
            for (int i = 0; i < 1; i++) {
                terminal_writestring("--");
                terminal_writestring(write_options[i]);

                /*
                if (i != 1) {
                    terminal_writestring(", ");
                }
                */
            }

            terminal_putchar('\n');

            terminal_writestring("cat       ");
            for (int i = 0; i < 2; i++) {
                terminal_writestring("--");
                terminal_writestring(cat_options[i]);

                if (i != 1) {
                    terminal_writestring(", ");
                }
            }

            terminal_putchar('\n');
            terminal_writestring("--------------------");
            terminal_putchar('\n');

            terminal_writestring("clear     NO OPTIONS\n");
            terminal_writestring("version   NO OPTIONS\n");
            terminal_writestring("reboot    NO OPTIONS\n");
            terminal_writestring("panic     NO OPTIONS\n");
            terminal_writestring("echo      NO OPTIONS\n");
            terminal_writestring("touch     NO OPTIONS\n");
    } else {
        terminal_writestring("List of commands:\n");
        terminal_writestring("-----------------\n");
        terminal_writestring("help\n");
        terminal_writestring("clear\n");
        terminal_writestring("version\n");
        terminal_writestring("reboot\n");
        terminal_writestring("panic\n");
        terminal_writestring("echo\n");
        terminal_writestring("ls\n");
        terminal_writestring("touch\n");
        terminal_writestring("write\n");
        terminal_writestring("cat\n");
        terminal_writestring("rm\n");
        terminal_writestring("-----------------\n");
        terminal_writestring("Type 'help --options' to see options for each command\n");
    }
}
