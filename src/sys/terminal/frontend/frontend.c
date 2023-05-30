

#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/pmmngr/alloc.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/memory.h>

Xtf* XtfInit(uint32_t virtual_height)
{
    Xtf* XtFrontend = (Xtf*)calloc(sizeof(XtFrontend)); // all variables = zeros    

    XtFrontend->vwidth = VGA_WIDTH; // 80
    XtFrontend->vheight = virtual_height;
    XtFrontend->buffer = (terminal_cell*)calloc(XtFrontend->vwidth * XtFrontend->vheight * sizeof(terminal_cell));
    XtFrontend->current_height = VGA_HEIGHT; 

    return XtFrontend;
}

void XtfDestroy(Xtf* XtFrontend)
{
    free(XtFrontend);
}

// inline static uint32_t XtfCursorPositionGet(Xtf* XtFrontend)
// {
//     if(XtFrontend == NULL)
//         return 0;
//     return (XtFrontend->y * XtFrontend->vwidth) + XtFrontend->x;
// }

void XtfCharacterPut(Xtf* XtFrontend, char c)
{

    XtFrontend->x++;
    
    if(c == NEW_LINE)
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;
    }
    
    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(DEFAULT_COLOR);

    if(XtFrontend->x >= XtFrontend->vwidth)
        XtfCharacterPut(XtFrontend, '\n');

}


void XtfCellPut(Xtf* XtFrontend, char c, uint8_t color)
{
    XtFrontend->x++;

    if(c == NEW_LINE)
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;
    }

    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(color);

    if(XtFrontend->x >= XtFrontend->vwidth)
        XtfCharacterPut(XtFrontend, '\n');

    if(XtbGet()->vga_height < XtFrontend->y + (XtFrontend->x / XtbGet()->vga_width))
        XtbScrollDown(XtFrontend);

}
