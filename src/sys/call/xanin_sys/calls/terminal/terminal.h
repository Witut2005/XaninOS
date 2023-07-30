
#pragma once

typedef uint8_t color_t;
typedef uint16_t terminal_cell;

struct Xtb{
    uint32_t y;             // real y position
    uint32_t vga_height;    // screen height
    uint32_t vga_width;     // screen width
    uint16_t* vram;         // ptr to text memory
};

typedef struct Xtb Xtb;

struct XtfCursor{
    bool is_used;
    int position;
    terminal_cell saved_cell;
    color_t color;
};

typedef struct XtfCursor XtfCursor;

struct Xtf
{
    uint32_t x;         // virtual x 
    uint8_t* rows_changed; 
    // uint32_t x_when_last_flushed;
    uint32_t y_begin;
    uint32_t y;         // virtual y
    // uint32_t y_when_last_flushed;
    uint32_t vwidth;
    uint32_t vheight;   // how many rows
    uint32_t current_height;
    terminal_cell* buffer;
    uint32_t size;
    uint32_t size_allocated;

    uint32_t x_screen;
    uint32_t y_screen;
    XtfCursor Cursor;
    bool scrolling_enabled;
};

typedef struct Xtf Xtf;


enum XANIN_TERMINAL_CURSOR_POSTIONS{
    CURSOR_POSITION_END = -1
};

enum XANIN_TERMINAL_SPECIAL_CHARACTERS{
    NEW_LINE = '\n',
    SAFE_NEW_LINE = '\x1e'
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

void vty_set(Xtf* XtFrontend) __attribute__((fastcall));
Xtf* vty_get(void);
void xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)__attribute__((fastcall));

#ifdef __cplusplus
}
#endif