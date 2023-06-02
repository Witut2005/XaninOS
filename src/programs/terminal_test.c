
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <lib/libc/stdiox.h>

int terminal_test(void)
{


    // xtb_init(VGA_WIDTH, VGA_HEIGHT, (uint16_t *)VGA_TEXT_MEMORY);

    // Xtf* Frontend = XtfInit(50);

    // xprintf("0x%x\n", Frontend);
    // xprintf("0x%x\n", Frontend->buffer);

    // xprintf("%d\n", Frontend->vwidth);
    // xprintf("%d\n", Frontend->height);

    // getchar();

    // for(int i = 0; i < Frontend->vwidth * 15 * 2; i++)
    //     xtf_cell_put(Frontend, 'a' + (i / Frontend->vwidth), OUTPUT_COLOR_SET(black, green));
    
    char tmp[] = "a";

    // putst("fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini fromini");

    // for(int i = 0; i < 29; i++)
    // {
    //     tmp[0] = 'a' + i;
    //     putst(tmp);
    // }

    for(int i = 0; i < 29; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            // xtf_character_put(stdio_vty_get(), 'a' + i);
            tmp[0] = 'a' + i;
            putst(tmp);
        }
        // xtf_character_put(stdio_vty_get(), '\n');
        putst("\n");
    }
    
    xtb_flush(stdio_vty_get());

    while(KeyInfo.scan_code != ENTER)
    {
        switch(KeyInfo.scan_code)
        {
            case ARROW_UP: 
            {
                xtb_scroll_up(stdio_vty_get());
                KeyInfo.scan_code = NULL;
                break;
            }
            case ARROW_DOWN:
            {

                xtb_scroll_down(stdio_vty_get());
                KeyInfo.scan_code = NULL;
                break;
            }
        }
    }

    // XtfFlush(Frontend);

}