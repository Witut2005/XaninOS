
#include <lib/stdiox.h>
#include <lib/math.h>

void hexview(void)
{
    KEYBOARD_SIG_OFF();

    no_enter = true;

    xprintf("\raddress number:\n");

    keyboard_scan_code = 0x0;

    while(1)
    {

        if(!index && keyboard_scan_code == ENTER)
        {
        

            char* data_ptr = (char*)atoi(COMMAND);
            clearScr();
            for(int i = 0; i < 0x20; i++)
                xprintf("%x",data_ptr[i]);
            return;      
            
        }
    }

}