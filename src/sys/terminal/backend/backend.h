
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

Xtb* xtb_get(void);
void xtb_flush(Xtf* XtFrontend);
void xtb_scroll_up(Xtf* XtFrontend);
void xtb_scroll_down(Xtf* XtFrontend);
void xtb_flush(Xtf* XtFrontend);
void xtb_cell_put(Xtf* XtFrontend, char c, uint8_t color);
void xtb_cursor_inc(Xtf* XtFrontend);
void xtb_cursor_dec(Xtf* XtFrontend);

static inline void xtb_character_put(Xtf* XtFrontend, char c)
{
    xtb_cell_put(XtFrontend, c, DEFAULT_COLOR);
}


#ifdef __cplusplus
}
#endif
