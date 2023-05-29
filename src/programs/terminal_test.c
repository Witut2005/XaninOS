
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>

int terminal_test(void)
{


    XtbInit(VGA_WIDTH, VGA_HEIGHT, (uint16_t *)VGA_TEXT_MEMORY);

    Xtf* Frontend = XtfInit(50);

    // xprintf("0x%x\n", Frontend);
    // xprintf("0x%x\n", Frontend->buffer);

    // xprintf("%d\n", Frontend->vwidth);
    // xprintf("%d\n", Frontend->height);

    // getchar();

    for(int i = 0; i < Frontend->vwidth * 15 * 2; i++)
        XtfCellPut(Frontend, 'a' + (i / Frontend->vwidth), OUTPUT_COLOR_SET(black, green));

    XtfFlush(Frontend);


    while(KeyInfo.scan_code != ENTER)
    {
        switch(KeyInfo.scan_code)
        {
            case ARROW_UP: 
            {
                XtbScrollUp(Frontend);
                KeyInfo.scan_code = NULL;
                break;
            }
            case ARROW_DOWN:
            {

                XtbScrollDown(Frontend);
                KeyInfo.scan_code = NULL;
                break;
            }
        }
        Screen.x = Screen.y = 0;
        xprintf("y: %d\n", Frontend->y);
        xprintf("y: %d\n", Frontend->current_height);
        xprintf("y: %d\n", Frontend->y_begin);
        xprintf("buf: 0x%x\n", Frontend->buffer);
    }

    // XtfFlush(Frontend);

}