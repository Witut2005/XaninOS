
#include <sys/terminal/backend/backend.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>

void xtb_scroll_up(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    XtFrontend->y_begin--;
    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin) * sizeof(terminal_cell);

    if(start_index == XT_NO_SUCH_LINE)
        XtFrontend->y_begin = 0;

    xtb_flush(XtFrontend);

}

void xtb_scroll_down(Xtf* XtFrontend)
{

    if(!XtFrontend->scrolling_enabled)
        return;

    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, (XtFrontend->y_begin++) + xtb_get()->vga_height);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + xtb_get()->vga_height) * sizeof(terminal_cell);

    if(start_index == XT_NO_SUCH_LINE)
        XtFrontend->y_begin = XtFrontend->current_height - xtb_get()->vga_height;
    
    xtb_flush(XtFrontend);

}

void xtb_flush(Xtf* XtFrontend)
{
    screen_buffer_clear();

    int vram_index = 0;
    XtFrontend->x_screen = XtFrontend->y_screen = 0;

    for(int i = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin); i < XtFrontend->size; i++)
    {
        if((char)(XtFrontend->buffer[i]) == SAFE_NEW_LINE)
        {
            if((!(vram_index % xtb_get()->vga_width)) && ((char)XtFrontend->buffer[i-1] != SAFE_NEW_LINE))
            {
                XtFrontend->y_screen++;
                XtFrontend->x_screen = 0;
                continue;
            }

            XtFrontend->y_screen++;
            XtFrontend->x_screen = 0;
            vram_index = vram_index + (xtb_get()->vga_width - (vram_index % xtb_get()->vga_width));
            continue;

        }

        else if((char)(XtFrontend->buffer[i]) == NEW_LINE)
        {
            
            vram_index = vram_index + (xtb_get()->vga_width - (vram_index % xtb_get()->vga_width));
            XtFrontend->y_screen++;
            XtFrontend->x_screen = 0;
            continue;
        }

        XtFrontend->x_screen++;
        uint16_t* vram = (uint16_t*)VGA_TEXT_MEMORY;
        vram[vram_index] = XtFrontend->buffer[i];

        if((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == i)) 
            vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);
        
        vram_index++;
    }

    if((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == CURSOR_POSITION_END)) 
    {
        uint16_t* vram = (uint16_t*)VGA_TEXT_MEMORY;
        vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);
    }

}

void xtb_cell_put(Xtf* XtFrontend, char c, uint8_t color)
{
    // XtFrontend->x++;

    if(XtFrontend->size + SECTOR_SIZE > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }

    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(color);
    XtFrontend->Cursor.position = CURSOR_POSITION_END;
    XtFrontend->x++;

    if(XtFrontend->x >= XtFrontend->vwidth)
    {
        XtFrontend->buffer[XtFrontend->size++] = SAFE_NEW_LINE | AS_COLOR(color);
        c = SAFE_NEW_LINE;
    }

    if((c == NEW_LINE) || (c == SAFE_NEW_LINE)) 
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;

        if(xtb_get()->vga_height - 3 < XtFrontend->y)
            xtb_scroll_down(XtFrontend);
    }

}

void xtb_cursor_inc(Xtf* XtFrontend)
{

    if(XtFrontend->Cursor.position == CURSOR_POSITION_END)
        return;

    XtFrontend->Cursor.position++;

    if(XtFrontend->Cursor.position >= XtFrontend->size)
        XtFrontend->Cursor.position = CURSOR_POSITION_END;

    xtb_flush(XtFrontend);
}

void xtb_cursor_dec(Xtf* XtFrontend)
{
    if(XtFrontend->Cursor.position == CURSOR_POSITION_END)
        XtFrontend->Cursor.position = XtFrontend->size;

    XtFrontend->Cursor.position--;
    xtb_flush(XtFrontend);
}
