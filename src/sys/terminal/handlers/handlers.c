
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/handlers/handlers.h>

void xtf_overflow_x_handler(Xtf* XtFrontend, char c, color_t color)
{
    if (XtFrontend->x >= XtFrontend->vwidth)
        XtFrontend->buffer[XtFrontend->size++].cell = SAFE_NEW_LINE | AS_COLOR(black);
}

void xt_cell_put_line_modifiers_handler(Xtf* XtFrontend, char c, color_t color)
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
}

bool xtf_handle_x_overflow(xtf_handler handler, Xtf* XtFrontend)
{
    if(XtFrontend->x >= XtFrontend->vwidth)
    {
        handler(XtFrontend, (char)XT_FUNCTION_ARGUMENT_NOT_USED, (color_t)XT_FUNCTION_ARGUMENT_NOT_USED);
        return true;
    }
    return false;
}

bool xt_handle_cell_put_line_modifires(xtf_handler handler, Xtf* XtFrontend, char c)
{
    if ((c == NEW_LINE) || (c == SAFE_NEW_LINE) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        handler(XtFrontend, c, XT_FUNCTION_ARGUMENT_NOT_USED);
        return true;
    }
    return false;
}

bool xt_cell_put_special_characters_handler(Xtf* XtFrontend, char c, color_t color) // you must here handle vwidth etc...
{
    if(c == ASCII_CR)
    { 
        XtFrontend->x = 0;
        XtFrontend->size = xtf_buffer_nth_line_index_get(XtFrontend, xtf_get_line_number_from_position(XtFrontend, XtFrontend->size)); 
        return true;
    }

    else if(c == ASCII_VT)
    {
        XtFrontend->x = 0;

        for(int i = 0; i < 3; i++, ++XtFrontend->y)
        {
            XtFrontend->rows_changed[XtFrontend->y] = true;
            if (XtFrontend->y >= xtb_get()->vga_height) 
                xtb_scroll_down(XtFrontend);
        }

        return true;
    }

    // else if(c == ASCII_TAB)
    // {
    //     for(int i = 0; i < 3; i++)
    //         xtb_cell_put(XtFrontend, ' ', color);
        
    //     return true;
    // }

    return false;
}

void xt_flush_special_characters_handle(char character, color_t color, uint32_t* current_row_to_display, bool* row_cleared, uint32_t* vram_index)
{
    
    Xtb* XtBackend = xtb_get();
    vga_screen_cell_t* vram = (vga_screen_cell_t*)VGA_TEXT_MEMORY;

    if (character == SAFE_NEW_LINE)
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

        //clear rows

        for(int i = 0; i < 3; i++)
        {
            for(int start_vram_index = *vram_index; *vram_index < start_vram_index + (XtBackend->vga_width - (start_vram_index % XtBackend->vga_width)); *vram_index += 1)
                vram[*vram_index] = BLANK_SCREEN_CELL;
        }

    }

}
