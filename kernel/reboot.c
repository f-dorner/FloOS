#include "reboot.h"
#include "io.h"

void kernel_reboot(void)
{
    outb(0x64, 0xFE);
}