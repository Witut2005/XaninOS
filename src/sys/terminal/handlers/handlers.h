
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

#ifdef __cplusplus
}
#endif