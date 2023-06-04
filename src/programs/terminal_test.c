
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <lib/libc/stdiox.h>

int terminal_test(void)
{

    char tmp[] = "a\n";

    // for(int i = 0; i < 29; i++)
    // {
    //     tmp[0] = 'a' + i;
    //     putct(tmp, (rand() % 10) + 2);
    // }

    char fro[100] = {'\0'};
    xscanft("%s", fro);

    for(int i = 0; i < 30; i++)
        xprintft("%s\n", fro);

    // xtb_flush(vty_get());

    while(inputg().scan_code != ENTER)
    {
        switch(KeyInfo.scan_code)
        {
            case ARROW_UP: 
            {
                xtb_scroll_up(vty_get());
                break;
            }
            case ARROW_DOWN:
            {

                xtb_scroll_down(vty_get());
                break;
            }
        }

        KeyInfo.scan_code = 0;

    }

    return XANIN_OK;

}