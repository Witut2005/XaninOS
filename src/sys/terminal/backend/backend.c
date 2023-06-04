
#include <sys/terminal/backend/backend.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>

static Xtb* XtBackend;

void xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)
{
    XtBackend = (Xtb*)calloc(sizeof(XtBackend));
    XtBackend->vga_width = vga_width;
    XtBackend->vga_height = vga_height;
    XtBackend->vram = vram;
}

Xtb* xtb_get(void)
{
    return XtBackend;
}

void xtb_scroll_up(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    XtFrontend->y_begin--;
    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin) * sizeof(terminal_cell);

    if(start_index == XT_NO_SUCH_LINE)
    {
        XtFrontend->y_begin = 0;
        return;
    }

    memmove((uint8_t*)VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), (uint8_t*)VGA_TEXT_MEMORY , XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));
    memcpy((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)&XtFrontend->buffer[start_index], 
        number_of_bytes_to_copy == -1 ? 0 : number_of_bytes_to_copy);

}

void xtb_scroll_down(Xtf* XtFrontend)
{
    if((XtBackend->vga_height < XtFrontend->vheight) && (XtFrontend->y_begin + XtBackend->vga_height) < XtFrontend->current_height)
    {
        int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);
        int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height) * sizeof(terminal_cell);
        XtFrontend->y_begin++;

        if(start_index == XT_NO_SUCH_LINE)
        {
            XtFrontend->y_begin = XtFrontend->current_height - XtBackend->vga_height;
            return;
        }

        memmove((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), XtBackend->vga_width * (XtBackend->vga_height - 1) * sizeof(terminal_cell));

        memcpy((uint8_t*)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * sizeof(terminal_cell)), 
            (uint8_t*)&XtFrontend->buffer[start_index], number_of_bytes_to_copy == -1 ? 0 : number_of_bytes_to_copy);

    }
}

void xtb_flush(Xtf* XtFrontend)
{
    screen_clear();

    int vram_index = 0;

    for(int i = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin); i < XtFrontend->size; i++)
    {
        if((char)(XtFrontend->buffer[i]) == NEW_LINE){
            vram_index = vram_index + (XtBackend->vga_width - (vram_index % XtBackend->vga_width));
            continue;
        }

        uint16_t* vram = (uint16_t*)VGA_TEXT_MEMORY;
        vram[vram_index++] = XtFrontend->buffer[i];
    }

}

void xtb_cell_put(Xtf* XtFrontend, char c, uint8_t color)
{
    XtFrontend->x++;

    if(XtFrontend->size + SECTOR_SIZE > XtFrontend->size_allocated)
    {
        XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size + SECTOR_SIZE);
        XtFrontend->size_allocated = XtFrontend->size + SECTOR_SIZE;
    }

    XtFrontend->buffer[XtFrontend->size++] = c | AS_COLOR(color);
        
    if((c == NEW_LINE) || (XtFrontend->x >= XtFrontend->vwidth))
    {
        XtFrontend->y++; 
        XtFrontend->x = 0;

        if(XtFrontend->y > XtFrontend->current_height)
            XtFrontend->current_height = XtFrontend->y;

        if(xtb_get()->vga_height < XtFrontend->y)
            xtb_scroll_down(XtFrontend);
    }

}