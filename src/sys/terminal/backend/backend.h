
#pragma once

#include <stdint.h>
#include <sys/terminal/frontend/frontend.h>
#include <lib/libc/stdiox.h>

struct Xtb{
    uint32_t y;             // real y position
    uint32_t vga_height;    // screen height
    uint32_t vga_width;     // screen width
    uint16_t* vram;         // ptr to text memory
};

typedef struct Xtb Xtb;

#ifdef __cplusplus
extern "C" {
#endif

void xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram);
Xtb* xtb_get(void);
// void XtbRowRemove(Xtf* Terminal);
// void XtbRowAdd(Xtf* XtFrontend);
void xtb_flush(Xtf* XtFrontend);
void xtb_scroll_up(Xtf* XtFrontend);
void xtb_scroll_down(Xtf* XtFrontend);
void xtb_flush(Xtf* XtFrontend);

#ifdef __cplusplus
}
#endif
