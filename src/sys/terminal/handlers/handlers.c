
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/handlers/handlers.h>

bool xtf_overflow_x_detect(Xtf* XtFrontend)
{
    if (XtFrontend->x >= XtFrontend->vwidth)
        return true;
    return false;
}


bool xt_cell_put_line_modifiers_handler(Xtf* XtFrontend, char c, color_t color) // also handles x overflow
{
    if ((c == NEW_LINE) || (c == XT_END_OF_ROW) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed
        XtFrontend->y++;
        XtFrontend->x = 0;

        if (XtFrontend->y > XtFrontend->current_height)
        {
            XtFrontend->current_height = XtFrontend->y;
            XtFrontend->rows_changed = (uint8_t *)realloc(XtFrontend->rows_changed, XtFrontend->y * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
        }

        if (XtFrontend->y >= xtb_get()->vga_height) 
            xtb_scroll_down(XtFrontend);

        return true;
    }
    return false;
}

bool xt_handle_cell_put_line_modifires(xtf_handler handler, Xtf* XtFrontend, char c)
{
    if ((c == NEW_LINE) || (c == XT_END_OF_ROW) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        handler(XtFrontend, c, XT_FUNCTION_ARGUMENT_NOT_USED);
        return true;
    }
    return false;
}

bool xt_cell_put_special_characters_handler(Xtf* XtFrontend, char c, color_t color) // you must here handle vwidth etc...
{

    if ((c == NEW_LINE) || (c == XT_END_OF_ROW))
    {
        XtFrontend->buffer[XtFrontend->size++].cell = c | AS_COLOR(color);
        XtFrontend->rows_changed[XtFrontend->y] = XTF_ROW_CHANGED; // mark current row as changed
        XtFrontend->y++;
        XtFrontend->x = 0;

        if (XtFrontend->y >= XtFrontend->current_height)
        {
            XtFrontend->current_height = XtFrontend->y + 1;
            XtFrontend->rows_changed = (uint8_t *)realloc(XtFrontend->rows_changed, XtFrontend->y * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
        }

        if (XtFrontend->y >= xtb_get()->vga_height) 
            xtb_scroll_down(XtFrontend);

        return true;
    }

    else if(c == ASCII_CR)
    { 
        XtFrontend->x = 0;
        XtFrontend->size = xtf_buffer_nth_line_index_get(XtFrontend, xtf_line_number_from_position_get(XtFrontend, XtFrontend->size)); 
        return true;
    }

    else if(c == ASCII_VT)
    {
        for(int i = 0; i < 3; i++)
            xtf_cell_put(XtFrontend, '\n', color);

        return true;
    }

    else if(c == ASCII_TAB)
    {
        for(int i = 0; i < 3; i++)
            xtf_cell_put(XtFrontend, ' ', color);
        
        return true;
    }

    return false;
}

void xt_flush_special_characters_handle(char character, color_t color, uint32_t* current_row_to_display, bool* row_cleared, uint32_t* vram_index)
{
    
    Xtb* XtBackend = xtb_get();
    vga_screen_cell_t* vram = (vga_screen_cell_t*)VGA_TEXT_MEMORY;

    if (character == XT_END_OF_ROW)
    {
        *current_row_to_display += 1;
        *row_cleared = false;

        if (!(*vram_index % XtBackend->vga_width))
            return;

        for(int start_vram_index = *vram_index; *vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); *vram_index += 1)
            vram[*vram_index] = BLANK_SCREEN_CELL;

    }

    else if (character == NEW_LINE)
    {
        *current_row_to_display += 1;
        *row_cleared = false;

        for(int start_vram_index = *vram_index; *vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); *vram_index += 1)
            vram[*vram_index] = BLANK_SCREEN_CELL;

    }

    else if(character == ASCII_TAB)
    {
        for(int i = 0; i < 3; i++, *vram_index += 1)
            vram[*vram_index] = ' ' | AS_COLOR(color);
    }

    else if(character == ASCII_VT)
    {
        *current_row_to_display += 3;
        *row_cleared = false;

        for(int i = 0; i < 3; i++)
        {
            for(int start_vram_index = *vram_index; *vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); *vram_index += 1)
                vram[*vram_index] = BLANK_SCREEN_CELL;
        }

    }

}
