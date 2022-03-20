
#include <lib/hal.h>
#include <lib/stdiox.h>


void shutdown(void)
{
    outdwIO(0x604, 0x2000);  /* QEMU */
    outdwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outdwIO(0x4004, 0x3400); /* VIRTUALBOX */

    screen_clear();

    xprintf("goodbye :))\n\n");

    xprintf("Your computer status:\n\n");
    reg_dump();

    asm("cli");
    asm("hlt");

}
