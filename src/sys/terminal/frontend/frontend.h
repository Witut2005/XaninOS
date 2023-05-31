
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint16_t terminal_cell;

struct Xtf
{
    uint32_t x;         // virtual x 
    uint32_t y_begin;
    uint32_t y;         // virtual y
    uint32_t vwidth;
    uint32_t vheight;   // how many rows
    uint32_t current_height;
    terminal_cell* buffer;
    uint32_t size;
};

typedef struct Xtf Xtf;

enum XANIN_TERMINAL_SPECIAL_CHARACTERS{
    NEW_LINE = '\n'
};

enum XANIN_TERMINAL_LINE{
    XT_NO_SUCH_LINE = -1
};

enum XANIN_TERMINAL_COLORS{
    DEFAULT_COLOR = 0x0F
};

#define AS_COLOR(x) (x << 8)

#ifdef __cplusplus
extern "C" {
#endif

Xtf* XtfInit(uint32_t virtual_height);
void XtfDestroy(Xtf* XtFrontend);
void XtfCharacterPut(Xtf* XtFrontend, char c);
void XtfCellPut(Xtf* XtFrontend, char c, uint8_t color);
int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0

#ifdef __cplusplus
}
#endif
