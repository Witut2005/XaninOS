

#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/pmmngr/alloc.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>

Xtf* XtfInit(uint32_t buffer_size)
{
    Xtf* XtFrontend = (Xtf*)calloc(sizeof(XtFrontend)); // all variables = zeros    

    XtFrontend->vwidth = VGA_WIDTH; // 80
    XtFrontend->vheight = 1000; //useless
    XtFrontend->size_allocated = buffer_size;
    XtFrontend->buffer = (terminal_cell*)calloc(buffer_size);
    XtFrontend->current_height = VGA_HEIGHT; 

    return XtFrontend;
}

void XtfDestroy(Xtf* XtFrontend)
{
    free(XtFrontend);
}

int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number)
{
    int index = xtf_buffer_nth_line_index_get(XtFrontend, line_number);
    int size = 0;

    while(((char)XtFrontend->buffer[index] != '\n') && ((char)XtFrontend->buffer[index] != '\0'))
    {
        size++;
        index++;
    }

    return size;

}

int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number) // starting with 0
{
    terminal_cell* buffer = XtFrontend->buffer;
    int current_line = 0;
    int index = 0;
 
    if(!line_number)
        return 0;

    while ((char)*buffer != '\0')
    {
        if ((char)*buffer == '\n')
        {
                current_line++;
                
                if (current_line + 1 > line_number)
                    break;
        }

        buffer++;
        index++;
    }

    if (current_line < line_number)
        return -1;
    
    return index + 1;
}


void xtf_character_put(Xtf* XtFrontend, char c)
{

    XtFrontend->x++;
    
    if(c == NEW_LINE)
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;
    }

    if(XtFrontend->size > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }
    
    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(DEFAULT_COLOR);

    if(XtFrontend->x >= XtFrontend->vwidth-1)
        xtf_character_put(XtFrontend, '\n');

}


void xtf_cell_put(Xtf* XtFrontend, char c, uint8_t color)
{
    XtFrontend->x++;

    if(c == NEW_LINE)
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;
    }

    if(XtFrontend->size > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }

    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(color);

    if(XtFrontend->x >= XtFrontend->vwidth)
        xtf_character_put(XtFrontend, '\n');

    if(xtb_get()->vga_height < XtFrontend->y + (XtFrontend->x / xtb_get()->vga_width))
        xtb_scroll_down(XtFrontend);

}
