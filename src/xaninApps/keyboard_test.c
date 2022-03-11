

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <handlers/handlers.c>

void keyboard_test(void)
{

    uint16_t* tmp = (uint16_t*)VGA_TEXT_MEMORY;

    clearScr();

    while(1)
    {
    

    xprintf("%x", key_info.scan_code);

    //if(key_pressed())
        //xprintf("%x ", ugalol);
        //*tmp = (uint16_t)('1'| ((black << 4) | white) << 8);
    //else
      //  *tmp = (uint16_t)('0'| ((black << 4) | white) << 8);

    }

}