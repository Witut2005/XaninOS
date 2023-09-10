
#include <lib/libc/hal.h>
#include <sys/input/input.h>

key_info_t KeyInfo = {0};

char inputc(void)
{
    // __xtb_flush(__vty_get());
    return inputg().character;
}


xchar inputg(void)
{

    // __xtb_flush(__vty_get());
    KeyInfo.scan_code = 0xFF;
    while(KeyInfo.scan_code > 0x80); // break codes doesnt count
    
    xchar x;

    x.character = KeyInfo.character;
    x.scan_code = KeyInfo.scan_code;

    return x;

}