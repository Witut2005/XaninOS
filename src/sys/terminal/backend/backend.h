
#pragma once

#include <stdint.h>
#include <lib/libc/stdiox.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/call/xanin_sys/calls/terminal/terminal.h>

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
void xtb_enable_flushing(void);
void xtb_disable_flushing(void);
void xtb_flush_all(Xtf* XtFrontend);
static inline void xtb_character_put(Xtf* XtFrontend, char c)
{
    xtb_cell_put(XtFrontend, c, DEFAULT_COLOR);
}


#ifdef __cplusplus
}
#endif
