
#include <sys/terminal/backend/backend.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>
#include <sys/terminal/vty/vty.h>
#include <sys/macros.h>

void xtb_scroll_up(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    XtFrontend->y_begin--;
    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin) * sizeof(terminal_cell);

    if(start_index == XT_NO_SUCH_LINE)
        XtFrontend->y_begin = 0;

    memmove((uint8_t*)VGA_TEXT_MEMORY + (xtb_get()->vga_width * sizeof(terminal_cell)), (uint8_t*)VGA_TEXT_MEMORY , xtb_get()->vga_width * xtb_get()->vga_height * sizeof(terminal_cell));
    memset((uint8_t*)VGA_TEXT_MEMORY, 0, xtb_get()->vga_width * sizeof(terminal_cell));

}

void xtb_scroll_down(Xtf* XtFrontend)
{

    // if(!XtFrontend->scrolling_enabled)
    //     return;

    // if((xtb_get()->vga_height < XtFrontend->vheight) && ((XtFrontend->y_begin + xtb_get()->vga_height - 1) <= XtFrontend->current_height) && (XtFrontend->y_begin > 0)) 
    if(XtFrontend->y > xtb_get()->vga_height)
    {
        int start_index = xtf_buffer_nth_line_index_get(XtFrontend, (XtFrontend->y_begin++) + xtb_get()->vga_height);
        int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + xtb_get()->vga_height) * sizeof(terminal_cell);

        if(start_index == XT_NO_SUCH_LINE)
            XtFrontend->y_begin = XtFrontend->current_height - xtb_get()->vga_height;
        
        memmove((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)VGA_TEXT_MEMORY + (xtb_get()->vga_width * sizeof(terminal_cell)), xtb_get()->vga_width * (xtb_get()->vga_height - 1) * sizeof(terminal_cell));
        memset((uint8_t*)VGA_TEXT_MEMORY + ((xtb_get()->vga_height - 1) * xtb_get()->vga_width * sizeof(terminal_cell)), 0, xtb_get()->vga_width * sizeof(terminal_cell));

}

void xtb_flush(Xtf* XtFrontend)
{
    if(!XtFrontend->size)
    {
        screen_buffer_clear();
    //     memset(XtFrontend->rows_changed, true, XtFrontend->current_height);
    // }

    int vram_index = 0;
    __vga_buffer_segment_get()[XtFrontend->cursor_vram_index] = BLANK_SCREEN_CELL;

    uint32_t current_row_to_display = XtFrontend->y_begin; //first row to display on screen
    bool row_cleared = false;

    for(int i = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin); i < XtFrontend->size; i++)
    {
        if((char)(XtFrontend->buffer[i]) == SAFE_NEW_LINE)
        {
            current_row_to_display++;
            row_cleared = false;

            if((!(vram_index % xtb_get()->vga_width)) && ((char)XtFrontend->buffer[i-1] != SAFE_NEW_LINE))
                continue;

            vram_index = vram_index + (xtb_get()->vga_width - (vram_index % xtb_get()->vga_width));
            continue;

        }

        else if((char)(XtFrontend->buffer[i]) == NEW_LINE)
        {
            current_row_to_display++;
            row_cleared = false;
            vram_index = vram_index + (xtb_get()->vga_width - (vram_index % xtb_get()->vga_width));
            continue;
        }

        uint16_t* vram = (uint16_t*)__vga_buffer_segment_get();
        
        if(XtFrontend->rows_changed[current_row_to_display])
        {
            if(!row_cleared)
            {
                for(int i = 0; i < XtFrontend->vwidth; i++)
                    vram[vram_index + i] = BLANK_SCREEN_CELL;
                row_cleared = true;
            }
            vram[vram_index] = XtFrontend->buffer[i];
        }

        if((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == i)) 
            vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);
        
        vram_index++;
    }

    for(; vram_index < xtb_get()->vga_height * xtb_get()->vga_width * sizeof(terminal_cell); vram_index++)
    {
        uint16_t* vram = (uint16_t*)__vga_buffer_segment_get();
        vram[vram_index] = BLANK_SCREEN_CELL;
    }

    if((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == CURSOR_POSITION_END)) 
    {
        uint16_t* vram = (uint16_t*)__vga_buffer_segment_get();
        vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);
        XtFrontend->cursor_vram_index = vram_index;
    }


    // for(int i = XtFrontend->y_begin; i < XtFrontend->current_height; i++) 
    // {
    //     if(XtFrontend->rows_changed[i])
    //         Screen.cursor[i][__vga_text_mode_width_get() - 1] = 0x4141;
    //     else
    //         Screen.cursor[i][__vga_text_mode_width_get() - 1] = 0x4242;

    // }

    memset((uint8_t*)XtFrontend->rows_changed, false, XtFrontend->current_height * sizeof(uint8_t));
}

void xtb_cell_put(Xtf* XtFrontend, char c, uint8_t color)
{
    if(XtFrontend->size + SECTOR_SIZE > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }

    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(color);


    XtFrontend->Cursor.position = CURSOR_POSITION_END;
    XtFrontend->x++;

    if(XtFrontend->x >= XtFrontend->vwidth)
        XtFrontend->buffer[XtFrontend->size++] = SAFE_NEW_LINE | AS_COLOR(color);

    if((c == NEW_LINE) || (c == SAFE_NEW_LINE) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        XtFrontend->rows_changed[XtFrontend->y] = true; // mark current row as changed
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
        {
            XtFrontend->current_height = XtFrontend->y;
            XtFrontend->rows_changed = (uint8_t*)realloc(XtFrontend->rows_changed, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
        }
        
        XtFrontend->rows_changed[XtFrontend->y] = true; // mark current row as changed

        if(xtb_get()->vga_height < XtFrontend->y)
            xtb_scroll_down(XtFrontend);
    }
    
    XtFrontend->rows_changed[XtFrontend->y] = true; // mark current row as changed

}

void xtb_cursor_inc(Xtf* XtFrontend)
{
    XANIN_DEBUG_RETURN();

    if(XtFrontend->Cursor.position == CURSOR_POSITION_END)
        return;

    XtFrontend->Cursor.position++;

    if(XtFrontend->Cursor.position >= XtFrontend->size)
        XtFrontend->Cursor.position = CURSOR_POSITION_END;

    XtFrontend->rows_changed[xtf_get_line_number_from_position(XtFrontend, XtFrontend->Cursor.position)] = true;
    xtb_flush(XtFrontend);
}

void xtb_cursor_dec(Xtf* XtFrontend)
{
    XANIN_DEBUG_RETURN();

    if(XtFrontend->Cursor.position == CURSOR_POSITION_END)
        XtFrontend->Cursor.position = XtFrontend->size;

    XtFrontend->Cursor.position--;
    XtFrontend->rows_changed[xtf_get_line_number_from_position(XtFrontend, XtFrontend->Cursor.position)] = true;
    xtb_flush(XtFrontend);
}
