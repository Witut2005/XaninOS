

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <handlers/handlers.c>

void keyboard_test(void)
{
    
    while(1)
    {
      xprintf("%x", KeyInfo.scan_code);
      if(Screen.y >= 30)
      {
        Screen.y = 0;
        Screen.x = 0;
      }

      if(getscan() == ESC)
        break;
      

    }

  app_exited = true;

}