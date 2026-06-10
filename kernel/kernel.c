#include <stdbool.h>
#include "terminal.h"
#include "keyboard.h"
#include "shell.h"
#include "fs.h"
#include "system_info.h"
#include "disk.h"

// Checking if the compiler thinks that the wrong os is targeted 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

//kernel only works for 32-bit ix86 targets 
#if !defined(__i386__)
#error "ix86-elf compiler is needed"
#endif

void kernel_main(void)
{
    //Initialize terminal interface

    terminal_initialize();

    system_print_boot_message();

    shell_init();
    shell_prompt();

    fs_init();

    while (1)
    {
        keyboard_poll();
    }
    
}