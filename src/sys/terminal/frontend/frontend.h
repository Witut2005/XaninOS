
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <lib/libc/memory.h>
#include <lib/ascii/ascii.h>
#include <lib/libc/stdlibx.h>
#include <sys/call/xanin_sys/calls/terminal/terminal.h>

#define XTF_ROW_CHANGED true
#define XTF_ROW_NOT_CHANGED false
#define XT_SIZE_OF_PARSED_CHARS 3

typedef void(*xtf_handler)(Xtf*, char, color_t);

#ifdef __cplusplus
extern "C" {
#endif

Xtf* xtf_init(uint32_t virtual_height);
void xtf_destroy(Xtf* XtFrontend);

int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int xtf_get_line_number_from_position(Xtf* XtFrontend, uint32_t position);

void xtf_remove_last_cell(Xtf* XtFrontend);
void xtf_virtual_cursor_add(Xtf* XtFrontend, color_t color);
void xtf_buffer_clear(Xtf* XtFrontend);

void xtf_scrolling_on(Xtf* XtFrontend);
void xtf_scrolling_off(Xtf* XtFrontend);

void xtf_cursor_on(Xtf* XtFrontend, color_t color);
void xtf_cursor_off(Xtf* XtFrontend);
void xtf_cursor_inc(Xtf *XtFrontend);
void xtf_cursor_dec(Xtf *XtFrontend);

static inline bool xtf_is_special_character(char c)
{
    return (c == ASCII_VT) | (c == ASCII_TAB) | (c == NEW_LINE) | (c == XT_END_OF_ROW);
}

static inline bool xt_is_parsed_character(char character)
{
    return (character == ASCII_VT) | (character == ASCII_TAB);
}


#ifdef __cplusplus
}
#endif
