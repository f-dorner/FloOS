#include "panic.h"
#include "terminal.h"

void kernel_panic(void)
{
    terminal_writestring("KERNEL PANIC\n");

    while (1) {
        asm volatile("hlt");
    }
    
}