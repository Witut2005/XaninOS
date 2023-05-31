
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

// void XtbRowRemove(Xtf* Terminal)
// {

// }

// void XtbRowAdd(Xtf* Terminal)
// {

// }

// uint32_t XtbRowSizeGet(Xtf* XtFrontend, uint32_t row)
// {

//     int index = 0;
//     int new_line_counter = 0;

//     while(new_line_counter <= row )
//     {
//         if(XtFrontend->size < index)
//             return 0;

//         if(XtFrontend->buffer[index] == '\n')
//             new_line_counter++;
//         index++;
//     }

//     return;

// }

// DO BACKENDU MUSI MIEC DOSTEP DO PRAWDZIWYCH WYMIAROW

void xtb_scroll_up(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    XtFrontend->y_begin--;
    int start_index = xtf_buffer_nth_line_index_get(XtFrontend, XtFrontend->y_begin);
    int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin);

    if(start_index == XT_NO_SUCH_LINE)
    {
        XtFrontend->y_begin++;
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
        int number_of_bytes_to_copy = xtf_buffer_nth_line_size_get(XtFrontend, XtFrontend->y_begin + XtBackend->vga_height);

        if(start_index == XT_NO_SUCH_LINE)
            return;

        memmove((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));

        memcpy((uint8_t*)VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * sizeof(terminal_cell)), 
            (uint8_t*)&XtFrontend->buffer[start_index], number_of_bytes_to_copy == -1 ? 0 : number_of_bytes_to_copy);

        XtFrontend->y_begin++;
    }
}

void xtb_flush(Xtf* XtFrontend)
{
    screen_clear();

    int vram_index = 0;

    for(int i = 0; i < XtFrontend->size; i++)
    {
        if((char)(XtFrontend->buffer[i]) == NEW_LINE){
            vram_index = vram_index + (XtBackend->vga_width - (vram_index % XtBackend->vga_width));
            continue;
        }
            
        uint16_t* vram = (uint16_t*)VGA_TEXT_MEMORY;
        vram[vram_index++] = XtFrontend->buffer[i];
    }

}