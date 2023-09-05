
#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>

#define XT_FUNCTION_ARGUMENT_NOT_USED 0

#ifdef __cplusplus
extern "C" {
#endif


void xtf_overflow_x_handler(Xtf* XtFrontend, char c, color_t color);
void xt_cell_put_line_modifiers_handler(Xtf* XtFrontend, char c, color_t color);
bool xtf_handle_x_overflow(xtf_handler handler, Xtf* XtFrontend);
bool xt_handle_cell_put_line_modifires(xtf_handler handler, Xtf* XtFrontend, char c);
bool xt_cell_put_special_characters_handler(Xtf* XtFrontend, char c, color_t color);
void xt_flush_special_characters_handle(char character, color_t color, uint32_t* current_row_to_display, bool* row_cleared, uint32_t* vram_index);

static inline bool xt_is_special_character(char character)
{
    return (character == NEW_LINE) | (character == SAFE_NEW_LINE) | (character == ASCII_TAB) | (character == ASCII_VT);
}

#ifdef __cplusplus
}
#endif