
#pragma once
#include <sys/macros.h>

typedef uint8_t color_t;

typedef uint16_t vga_screen_cell_t;

union XtCell 
{
    uint16_t cell;
    struct {
        char character;
        union {
            uint8_t color;
            struct  
            {
                uint8_t foreground : 4;
                uint8_t background : 4;
            }__attribute__((packed));
        };
    }__attribute__((packed));
};

typedef union XtCell XtCell; // should be struct

struct Xtb{
    uint32_t y;             // real y position
    uint32_t vga_height;    // screen height
    uint32_t vga_width;     // screen width
    uint16_t* vram;         // ptr to text memory
    bool is_flushable;
    bool is_currently_flushing;
};

typedef struct Xtb Xtb;

struct XtfCursor{
    bool is_used;
    int position;
    XtCell saved_cell;
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
    XtCell* buffer;
    uint32_t size;
    uint32_t size_allocated;
    uint32_t cursor_vram_index;

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
    XT_END_OF_ROW = '\x1e'
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

void __sys_vty_set(Xtf* XtFrontend);
Xtf* __sys_vty_get(void);
void __sys_xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram);

Xtb* __sys_xtb_get(void);
void __sys_xtb_flush(Xtf* XtFrontend);
void __sys_xtb_scroll_up(Xtf* XtFrontend);
void __sys_xtb_scroll_down(Xtf* XtFrontend);
void __sys_xtb_flush_all(Xtf* XtFrontend);

Xtf* __sys_xtf_init(uint32_t buffer_size);
void __sys_xtf_destroy(Xtf* XtFrontend);

int __sys_xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int __sys_xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number); // starting with 0
int __sys_xtf_line_number_from_position_get(Xtf* XtFrontend, uint32_t position);

void __sys_xtf_cell_put(Xtf *XtFrontend, char c, uint8_t color);
void __sys_xtf_character_put(Xtf* XtFrontend, char character);

void __sys_xtf_remove_last_cell(Xtf* XtFrontend);
void __sys_xtf_virtual_cursor_add(Xtf* XtFrontend, color_t color);
void __sys_xtf_buffer_clear(Xtf* XtFrontend);

void __sys_xtf_scrolling_on(Xtf* XtFrontend);
void __sys_xtf_scrolling_off(Xtf* XtFrontend);

void __sys_xtf_cursor_on(Xtf* XtFrontend, color_t color);
void __sys_xtf_cursor_off(Xtf* XtFrontend);
void __sys_xtf_cursor_inc(Xtf *XtFrontend);
void __sys_xtf_cursor_dec(Xtf *XtFrontend);

#ifdef __cplusplus
}
#endif