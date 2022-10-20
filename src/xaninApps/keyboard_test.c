

#include <libc/stdiox.h>
#include <libc/stdlibx.h>

int keyboard_test(void)
{
    
    while(1)
    {
        // xprintf("%x", inbIO(0x60));
        xprintf("%x", KeyInfo.scan_code); 
        if(Screen.y >= 30)
        {
            Screen.y = 0;
            Screen.x = 0;
        }

        if(KeyInfo.scan_code == ESC)
            exit();
    }
    return XANIN_OK;
}