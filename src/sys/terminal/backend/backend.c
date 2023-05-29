
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
    XtBackend = (Xtb*)kcalloc(sizeof(XtBackend));
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

void XtbFlush(Xtf* Terminal)
{

}

// DO BACKENDU MUSI MIEC DOSTEP DO PRAWDZIWYCH WYMIAROW

void XtbScrollUp(Xtf* XtFrontend)
{
    if(!XtFrontend->y_begin)
        return;

    memmove(VGA_TEXT_MEMORY, VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));

    memcpy(VGA_TEXT_MEMORY + (XtBackend->vga_width * (XtBackend->vga_height - 1) * sizeof(terminal_cell)), 
        &XtFrontend->buffer[(XtFrontend->y_begin + XtBackend->vga_height) * XtFrontend->vwidth], XtBackend->vga_width * sizeof(terminal_cell));

    XtFrontend->y_begin--;
}

void XtbScrollDown(Xtf* XtFrontend)
{
    if((XtBackend->vga_height < XtFrontend->vheight) && (XtFrontend->y_begin + XtBackend->vga_height) < XtFrontend->current_height)
    {
        memmove(VGA_TEXT_MEMORY + (XtBackend->vga_width * sizeof(terminal_cell)), VGA_TEXT_MEMORY, XtBackend->vga_width * XtBackend->vga_height * sizeof(terminal_cell));
        // memset(VGA_TEXT_MEMORY, 0, XtBackend->vga_width * sizeof(terminal_cell)); // nowy pierwszy razad z buffera
        memcpy(VGA_TEXT_MEMORY, &XtFrontend->buffer[(XtFrontend->y_begin-1) * XtFrontend->vwidth], XtBackend->vga_width * sizeof(terminal_cell));
        XtFrontend->y_begin++;
    }
}