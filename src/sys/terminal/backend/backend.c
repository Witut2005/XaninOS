
#include <sys/macros.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <sys/terminal/vty/vty.h>
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/handlers/handlers.h>

static Xtb* XtBackend;

Xtb* __xtb_get(void)
{
    return XtBackend;
}

void __xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)
{
    XtBackend = (Xtb*)kcalloc(SIZE_OF(Xtb));
    XtBackend->vga_width = vga_width;
    XtBackend->vga_height = vga_height;
    XtBackend->vram = vram;
    XtBackend->is_flushable = true;
}


// CURSOR DISABLED GLOBALLY
void __xtb_scroll_up(Xtf *XtFrontend)
{

    Xtb *XtBackend = __xtb_get();

    if ((!XtFrontend->scrolling_enabled) || (!XtBackend->is_flushable) || (!XtFrontend->y_begin))
        return;

    XtFrontend->y_begin--;
    int start_index = __xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = __xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin);

    if (start_index == XT_NO_SUCH_LINE)
    {
        XtFrontend->y_begin = 0;
        return;
    }

    xtb_disable_flushing(); // mutex like

    memmove((uint8_t *)VGA_TEXT_MEMORY + (XtBackend->vga_width * SIZE_OF(XtCell)), (uint8_t *)VGA_TEXT_MEMORY, XtBackend->vga_width * XtBackend->vga_height * SIZE_OF(XtCell)); // move terminal data

    memset((uint8_t *)VGA_TEXT_MEMORY, BLANK_SCREEN_CELL, XtBackend->vga_width * SIZE_OF(XtCell)); // clear row
    
    memcpy((uint8_t *)VGA_TEXT_MEMORY, (uint8_t *)&XtFrontend->buffer[start_index],               // display new line
           number_of_bytes_to_copy * SIZE_OF(XtCell));

    xtb_enable_flushing();

}

void __xtb_scroll_down(Xtf *XtFrontend)
{
    Xtb *XtBackend = __xtb_get();

    if ((!XtFrontend->scrolling_enabled) || (!XtBackend->is_flushable))
        return;

    if ((XtFrontend->y >= XtBackend->vga_height) && (XtFrontend->y_begin + XtBackend->vga_height <= XtFrontend->current_height))
    {
        int start_index = __xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);
        int number_of_cells_to_copy = __xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);

        if (start_index == XT_NO_SUCH_LINE)
            return;

        xtb_disable_flushing(); // mutex like

        XtFrontend->y_begin++;

        memmove((uint8_t *)VGA_TEXT_MEMORY, (uint8_t *)VGA_TEXT_MEMORY + (__xtb_get()->vga_width * SIZE_OF(XtCell)), __xtb_get()->vga_width * (__xtb_get()->vga_height - 1) * SIZE_OF(XtCell)); // move terminal data

        memset((uint8_t *)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * SIZE_OF(XtCell)), BLANK_SCREEN_CELL, XtBackend->vga_width * SIZE_OF(XtCell)); // clear row

        memcpy((uint8_t *)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * SIZE_OF(XtCell)),                                                            // display new line
               (uint8_t *)&XtFrontend->buffer[start_index], number_of_cells_to_copy * SIZE_OF(XtCell));

        xtb_enable_flushing();
    }
}

void __xtb_flush(Xtf *XtFrontend)
{

    Xtb *XtBackend = __xtb_get();

    if (!XtBackend->is_flushable)
        return;

    if(XtBackend->is_currently_flushing) // is already flushing (probably it is interval)
        return;

    if (!XtFrontend->size) // empty buffer
    {
        vga_screen_buffer_clear();
        memset(XtFrontend->rows_changed, XTF_ROW_CHANGED, XtFrontend->current_height);
        return;
    }

    XtBackend->is_currently_flushing = true;

    uint32_t vram_index = 0;
    uint32_t current_row_to_display = XtFrontend->y_begin; // first row to display on screen
    bool row_cleared = false;

    uint16_t *vram = (uint16_t *)VGA_TEXT_MEMORY;

    for (int i = __xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin); i < XtFrontend->vheight * XtFrontend->vwidth; i++)
    {

        if(i >= XtFrontend->size_allocated)
            break;

        if(xt_is_special_character(XtFrontend->buffer[i].character))
            xt_flush_special_characters_handle(XtFrontend->buffer[i].character, XtFrontend->buffer[i].color, &current_row_to_display, &row_cleared, &vram_index);
        
        else
        {
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
            vram_index++;
        }

    }

    vram_index++;

    X86_POINTER vram_to_clear;

    for (; vram_index < XtBackend->vga_height * XtBackend->vga_width * SIZE_OF(XtCell) / (X86_POINTER_SIZE / SIZE_OF(XtCell)); vram_index++)
        vram_to_clear[vram_index] = BLANK_SCREEN_CELL;

    memset((uint8_t *)XtFrontend->rows_changed, XTF_ROW_NOT_CHANGED, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
    XtBackend->is_currently_flushing = false;
}

void __xtb_flush_all(Xtf *XtFrontend)
{
    memset(XtFrontend->rows_changed, XTF_ROW_CHANGED, XtFrontend->current_height * SIZE_OF_POINTED_TYPE(XtFrontend->rows_changed));
    __xtb_flush(XtFrontend);
}