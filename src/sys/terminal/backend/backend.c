
#include <sys/macros.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <sys/terminal/vty/vty.h>
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/handlers/handlers.h>

void xtb_scroll_up(Xtf *XtFrontend)
{

    Xtb *XtBackend = xtb_get();

    if ((!XtFrontend->scrolling_enabled) || (!XtBackend->is_flushable) || (!XtFrontend->y_begin))
        return;

    XtFrontend->y_begin--;
    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin);

    if (start_index == XT_NO_SUCH_LINE)
    {
        XtFrontend->y_begin = 0;
        return;
    }

    xtb_disable_flushing();

    memmove((uint8_t *)VGA_TEXT_MEMORY + (XtBackend->vga_width * SIZE_OF(XtCell)), (uint8_t *)VGA_TEXT_MEMORY, XtBackend->vga_width * XtBackend->vga_height * SIZE_OF(XtCell)); // move terminal data

    memset((uint8_t *)VGA_TEXT_MEMORY, BLANK_SCREEN_CELL, XtBackend->vga_width * SIZE_OF(XtCell)); // clear row
    memcpy((uint8_t *)VGA_TEXT_MEMORY, (uint8_t *)&XtFrontend->buffer[start_index],               // display new line
           number_of_bytes_to_copy * SIZE_OF(XtCell));

    xtb_enable_flushing();

}

void xtb_scroll_down(Xtf *XtFrontend)
{
    Xtb *XtBackend = xtb_get();

    if ((!XtFrontend->scrolling_enabled) || (!XtBackend->is_flushable))
        return;

    if ((XtFrontend->y >= XtBackend->vga_height) && (XtFrontend->y_begin + XtBackend->vga_height <= XtFrontend->current_height))
    {
        int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);
        int number_of_cells_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);

        if (start_index == XT_NO_SUCH_LINE)
            return;

        xtb_disable_flushing();

        XtFrontend->y_begin++;

        memmove((uint8_t *)VGA_TEXT_MEMORY, (uint8_t *)VGA_TEXT_MEMORY + (xtb_get()->vga_width * SIZE_OF(XtCell)), xtb_get()->vga_width * (xtb_get()->vga_height - 1) * SIZE_OF(XtCell)); // move terminal data

        memset((uint8_t *)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * SIZE_OF(XtCell)), BLANK_SCREEN_CELL, XtBackend->vga_width * SIZE_OF(XtCell)); // clear row
        memcpy((uint8_t *)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * SIZE_OF(XtCell)),                                                            // display new line
               (uint8_t *)&XtFrontend->buffer[start_index], number_of_cells_to_copy * SIZE_OF(XtCell));

        xtb_enable_flushing();

    }
}

void xtb_flush(Xtf *XtFrontend)
{

    Xtb *XtBackend = xtb_get();

    if (!XtBackend->is_flushable)
        return;

    if(XtBackend->is_currently_flushing)
        return;

    if (!XtFrontend->size)
    {
        vga_screen_buffer_clear();
        memset(XtFrontend->rows_changed, XTF_ROW_CHANGED, XtFrontend->current_height);
        return;
    }

    XtBackend->is_currently_flushing = true;

    int vram_index = 0;
    uint32_t current_row_to_display = XtFrontend->y_begin; // first row to display on screen
    bool row_cleared = false;

    uint16_t *vram = (uint16_t *)VGA_TEXT_MEMORY;

    for (int i = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin); i < XtFrontend->vheight * XtFrontend->vwidth; i++)
    {

        if(i >= XtFrontend->size_allocated)
            break;

        if (XtFrontend->buffer[i].character == SAFE_NEW_LINE)
        {
            current_row_to_display++;
            row_cleared = false;

            if (!(vram_index % XtBackend->vga_width))// && (XtFrontend->buffer[i - 1].character != SAFE_NEW_LINE))
                continue;

            for(int start_vram_index = vram_index; vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); vram_index++)
                vram[vram_index] = BLANK_SCREEN_CELL;

            continue;
        }

        else if (XtFrontend->buffer[i].character == NEW_LINE)
        {
            current_row_to_display++;
            row_cleared = false;

            for(int start_vram_index = vram_index; vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); vram_index++)
                vram[vram_index] = BLANK_SCREEN_CELL;

            continue;
        }

        if (XtFrontend->rows_changed[current_row_to_display])
        {
            if (!row_cleared)
            {
                for (int j = 0; j < XtFrontend->vwidth; j++)
                    vram[vram_index + j] = BLANK_SCREEN_CELL;
                row_cleared = true;
            }
            vram[vram_index] = XtFrontend->buffer[i].cell;
        }

        if ((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == i))
            vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);

        vram_index++;
    }

    if ((XtFrontend->Cursor.is_used) && (XtFrontend->Cursor.position == CURSOR_POSITION_END))
    {
        uint16_t *vram = (uint16_t *)VGA_TEXT_MEMORY;
        vram[vram_index] = (char)vram[vram_index] | AS_COLOR(XtFrontend->Cursor.color);
        XtFrontend->cursor_vram_index = vram_index;
    }

    vram_index++;

    for (; vram_index < XtBackend->vga_height * XtBackend->vga_width * SIZE_OF(XtCell); vram_index++)
        vram[vram_index] = BLANK_SCREEN_CELL;

    memset((uint8_t *)XtFrontend->rows_changed, XTF_ROW_NOT_CHANGED, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
    XtBackend->is_currently_flushing = false;
}


void xtb_flush_all(Xtf *XtFrontend)
{
    memset(XtFrontend->rows_changed, XTF_ROW_CHANGED, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
    xtb_flush(XtFrontend);
}

void xtb_cell_put_at_position(Xtf *XtFrontend, char c, uint8_t color, uint32_t position)
{

    Xtb *XtBackend = xtb_get();

    if(position >= XtFrontend->size)
        return;

    if (XtFrontend->size + SECTOR_SIZE > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (XtCell *)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }

    XtFrontend->buffer[position].cell = c | AS_COLOR(color);
    XtFrontend->Cursor.position = CURSOR_POSITION_END;

    if ((c == NEW_LINE) || (c == SAFE_NEW_LINE) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        XtFrontend->y++;
        XtFrontend->x = 0;

        if (XtFrontend->y > XtFrontend->current_height)
        {
            XtFrontend->current_height = XtFrontend->y;
            XtFrontend->rows_changed = (uint8_t *)realloc(XtFrontend->rows_changed, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
        }

        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed

        if (XtFrontend->y >= XtBackend->vga_height)
        {
            xtb_flush(XtFrontend);
            xtb_scroll_down(XtFrontend);
        }
    }
    else
        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed
}

void xtb_cell_put(Xtf *XtFrontend, char c, uint8_t color)
{

    Xtb *XtBackend = xtb_get();

    if (XtFrontend->size + 1 >= XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (XtCell *)realloc(XtFrontend->buffer, (XtFrontend->size + SECTOR_SIZE * 4) * SIZE_OF(XtCell));
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE * 4;
    }

    if(c == '\r')
    {
        XtFrontend->size = xtf_buffer_nth_line_index_get(XtFrontend, xtf_get_line_number_from_position(XtFrontend, XtFrontend->size)); 
        return;
    }

    else
        XtFrontend->buffer[XtFrontend->size++].cell = c | AS_COLOR(color);

    XtFrontend->Cursor.position = CURSOR_POSITION_END;
    XtFrontend->x++;

    xtf_handle_x_overflow(XtFrontend, xtf_overflow_x_handler);

    if ((c == NEW_LINE) || (c == SAFE_NEW_LINE) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        XtFrontend->y++;
        XtFrontend->x = 0;

        if (XtFrontend->y > XtFrontend->current_height)
        {
            XtFrontend->current_height = XtFrontend->y;
            XtFrontend->rows_changed = (uint8_t *)realloc(XtFrontend->rows_changed, XtFrontend->y * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
        }

        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed

        if (XtFrontend->y >= XtBackend->vga_height)
        {
            xtb_flush(XtFrontend);
            xtb_scroll_down(XtFrontend);
        }
    }
    else
        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed

    XtFrontend->buffer[XtFrontend->size].cell = '\0';

}
