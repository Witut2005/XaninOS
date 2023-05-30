
#include <sys/terminal/backend/backend.h>

static Xtb* XtBackend;

static inline uint32_t XtbCalculateY(Xtf* XtFrontend)
{
    if(XtFrontend->y >= XtBackend->vga_height)
        return XtBackend->y = XtBackend->vga_height - 1;
    else
        XtFrontend->y;
}

void XtbInit(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)
{
    XtBackend = (Xtb*)calloc(sizeof(XtBackend));
    XtBackend->vga_width = vga_width;
    XtBackend->vga_height = vga_height;
    XtBackend->vram = vram;
}


Xtb* XtbGet(void)
{
    return XtBackend;
}

void XtbRowRemove(Xtf* Terminal)
{

}

void XtbRowAdd(Xtf* Terminal)
{

}

uint32_t XtbRowSizeGet(Xtf* XtFrontend, uint32_t row)
{

    int index = 0;
    int new_line_counter = 0;

    while(new_line_counter < row - 1)
    {
        if(XtFrontend->size < index)
            return 0;

        if(XtFrontend->buffer[index] == '\n')
            new_line_counter++;
        index++;
    }

    return;

}

uint32_t XtbRowIndexGet(Xtf* XtFrontend, uint32_t row)
{

    int index = 0;
    int new_line_counter = 0;

    while(new_line_counter < row - 1)
    {
        if(XtFrontend->size < index)
            return 0;

        if(XtFrontend->buffer[index] == '\n')
            new_line_counter++;
        index++;
    }

    return;

}

// DO BACKENDU MUSI MIEC DOSTEP DO PRAWDZIWYCH WYMIAROW

void XtbScrollUp(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    memmove(VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), VGA_TEXT_MEMORY , XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));
    memcpy(VGA_TEXT_MEMORY, &XtFrontend->buffer[(--XtFrontend->y_begin) * XtFrontend->vwidth], XtbRow
}

void XtbScrollDown(Xtf* XtFrontend)
{
    if((XtBackend->vga_height < XtFrontend->vheight) && (XtFrontend->y_begin + XtBackend->vga_height) < XtFrontend->current_height)
    {
        memmove(VGA_TEXT_MEMORY, VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));
        memcpy(VGA_TEXT_MEMORY + ((XtBackend->vga_height - 1) * XtBackend->vga_width * sizeof(terminal_cell)), &XtFrontend->buffer[(XtFrontend->y_begin + XtBackend->vga_height) * XtFrontend->vwidth], XtBackend->vga_width * sizeof(terminal_cell));
        XtFrontend->y_begin++;
    }
}

void XtbFlush(Xtf* XtFrontend)
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

    // memcpy((uint8_t*)VGA_TEXT_MEMORY, (uint8_t*)&XtFrontend->buffer[XtFrontend->y_begin * XtFrontend->vwidth], VGA_SCREEN_RESOLUTION * sizeof(terminal_cell));    
}