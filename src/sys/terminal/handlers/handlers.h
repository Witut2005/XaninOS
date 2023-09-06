
#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>

#define XT_FUNCTION_ARGUMENT_NOT_USED 0

#ifdef __cplusplus
extern "C" {
#endif


bool xtf_overflow_x_handler(Xtf* XtFrontend);
bool xt_cell_put_line_modifiers_handler(Xtf* XtFrontend, char c, color_t color);
bool xtf_overflow_x_detect(Xtf* XtFrontend);
bool xt_handle_cell_put_line_modifires(xtf_handler handler, Xtf* XtFrontend, char c);
bool xt_cell_put_special_characters_handler(Xtf* XtFrontend, char c, color_t color);
void xt_flush_special_characters_handle(char character, color_t color, uint32_t* current_row_to_display, bool* row_cleared, uint32_t* vram_index);

static inline bool xt_is_special_character(char character)
{
    return (character == NEW_LINE) | (character == XT_END_OF_ROW) | (character == ASCII_TAB) | (character == ASCII_VT);
}

static inline bool xt_is_normal_character(char character) // returns true if character is not special
{
    return (character >= 0x20) & (character < 0x7F);
}

#ifdef __cplusplus
}
#endif