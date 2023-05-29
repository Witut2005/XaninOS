

#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/pmmngr/alloc.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/memory.h>

Xtf* XtfInit(uint32_t virtual_height)
{
    Xtf* XtFrontend = (Xtf*)kcalloc(sizeof(XtFrontend)); // all variables = zeros    

    XtFrontend->vwidth = VGA_WIDTH; // 80
    XtFrontend->vheight = virtual_height;
    XtFrontend->buffer = (terminal_cell*)kcalloc(XtFrontend->vwidth * XtFrontend->vheight * sizeof(terminal_cell));
    XtFrontend->current_height = VGA_HEIGHT; 

    return XtFrontend;
}

void XtfDestroy(Xtf* XtFrontend)
{
    free(XtFrontend);
}

inline static uint32_t XtfCursorPositionGet(Xtf* XtFrontend)
{
    if(XtFrontend == NULL)
        return 0;
    return (XtFrontend->y * XtFrontend->vwidth) + XtFrontend->x;
}

void XtfCharacterPut(Xtf* XtFrontend, char c)
{
    XtFrontend->buffer[XtfCursorPositionGet(XtFrontend)] = c | AS_COLOR(DEFAULT_COLOR);

    XtFrontend->x++;

    if(XtFrontend->x == XtFrontend->vwidth)
    {
        XtFrontend->x = 0;
        XtFrontend->y++;
        XtFrontend->current_height++;
    }
}

void XtfCellPut(Xtf* XtFrontend, char c, uint8_t color)
{
    XtFrontend->buffer[XtfCursorPositionGet(XtFrontend)] = c | AS_COLOR(color);
    XtFrontend->x++;

    if(XtFrontend->x == XtFrontend->vwidth)
    {
        XtFrontend->x = 0;
        XtFrontend->y++;
        
        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height++;

        if(XtbGet()->vga_height < XtFrontend->current_height - XtbGet()->vga_height)
        {
            XtbScrollDown(XtFrontend);
        }
    }


}

void XtfFlush(Xtf* XtFrontend)
{
    screen_clear();
    memcpy((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)&XtFrontend->buffer[XtFrontend->y_begin * XtFrontend->vwidth], VGA_SCREEN_RESOLUTION * sizeof(uint16_t));    
}
