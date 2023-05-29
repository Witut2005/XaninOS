
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>

int terminal_test(void)
{


    XtbInit(VGA_WIDTH, VGA_HEIGHT, (uint16_t *)VGA_TEXT_MEMORY);

    Xtf* Frontend = XtfInit(30);

    // xprintf("0x%x\n", Frontend);
    // xprintf("0x%x\n", Frontend->buffer);

    // xprintf("%d\n", Frontend->vwidth);
    // xprintf("%d\n", Frontend->height);

    // getchar();

    for(int i = 0; i < Frontend->vwidth * VGA_HEIGHT * 2; i++)
        XtfCellPut(Frontend, 'a' + (i / Frontend->vwidth), OUTPUT_COLOR_SET(black, green));

    XtfFlush(Frontend);


    while(KeyInfo.scan_code != ENTER)
    {
        switch(KeyInfo.scan_code)
        {
            case ARROW_UP: 
            {
                XtbScrollUp(Frontend);
                break;
            }
            case ARROW_DOWN:
            {

                XtbScrollDown(Frontend);
                break;
            }
        }
        Screen.x = Screen.y = 0;
        xprintf("y: %d", Frontend->y);
        KeyInfo.scan_code = NULL;
    }

    // XtfFlush(Frontend);

}