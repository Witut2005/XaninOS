
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

void XtbInit(uint32_t vga_width, uint32_t vga_height, uint16_t* vram);
Xtb* XtbGet(void);
void XtbRowRemove(Xtf* Terminal);
void XtbRowAdd(Xtf* XtFrontend);
void XtbFlush(Xtf* XtFrontend);
void XtbScrollUp(Xtf* XtFrontend);
void XtbScrollDown(Xtf* XtFrontend);
void XtbFlush(Xtf* XtFrontend);

#ifdef __cplusplus
}
#endif
