

#include <libc/stdiox.h>
#include <libc/stdlibx.h>

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

      if(KeyInfo.scan_code == ESC)
        break;
      

    }

  app_exited = true;

}