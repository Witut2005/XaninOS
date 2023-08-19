
#pragma once

#include <sys/terminal/frontend/frontend.h>
#include <sys/terminal/backend/backend.h>

static Xtf* CurrentVty; 
static Xtb* XtBackend;

void __vty_set(Xtf* XtFrontend)
{
    CurrentVty = XtFrontend;
}

Xtf* __vty_get(void)
{
    return CurrentVty;

}
Xtb* __xtb_get(void)
{
    return XtBackend;
}

void __xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)
{
    XtBackend = (Xtb*)kcalloc(sizeof(xtb_get()));
    XtBackend->vga_width = vga_width;
    XtBackend->vga_height = vga_height;
    XtBackend->vram = vram;
    XtBackend->is_flushable = true;
}