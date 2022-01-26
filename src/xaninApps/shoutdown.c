
#include <lib/hal.h>



void shoutdown(void)
{
    outdwIO(0x604, 0x2000);  /* QEMU */
    outdwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outdwIO(0x4004, 0x3400); /* VIRTUALBOX */
}