
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <sys/call/xanin_sys/calls/terminal/terminal.h>

#ifdef __cplusplus
extern "C" {
#endif

Xtf* xtf_init(uint32_t virtual_height);

void xtf_destroy(Xtf* XtFrontend);

int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
// void vty_set(Xtf* XtFrontend);
// Xtf* vty_get(void);
void xtf_remove_last_cell(Xtf* XtFrontend);
void xtf_virtual_cursor_add(Xtf* XtFrontend, color_t color);
void xtf_buffer_clear(Xtf* XtFrontend);
void xtf_cursor_on(Xtf* XtFrontend, color_t color);
void xtf_cursor_off(Xtf* XtFrontend);
void xtf_scrolling_on(Xtf* XtFrontend);
void xtf_scrolling_off(Xtf* XtFrontend);
int xtf_get_line_number_from_position(Xtf* XtFrontend, uint32_t position);


#ifdef __cplusplus
}
#endif
