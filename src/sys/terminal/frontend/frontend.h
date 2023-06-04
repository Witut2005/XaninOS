
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <lib/libc/stdlibx.h>

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
    uint32_t size_allocated;
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

Xtf* xtf_init(uint32_t virtual_height);

static inline void xtf_destroy(Xtf* XtFrontend)
{
    free(XtFrontend);
}

int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
void vty_set(Xtf* XtFrontend);
Xtf* vty_get(void);
void xtf_remove_last_cell(Xtf* XtFrontend);


#ifdef __cplusplus
}
#endif
