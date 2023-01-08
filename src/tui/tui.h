
#include <stdint.h>
#include <libc/stdiox.h>
#include <terminal/vty.h>

struct table_t
{
    uint16_t x;
    uint16_t y;
    uint16_t number_of_rows;
    uint16_t row_size;
    uint8_t background_color;
    uint8_t foreground_color;
    void (*handler)(char*);
};

struct ScreenCell
{
    uint8_t foreground_color;
    uint8_t background_color;
    char character;
};

#ifndef __cplusplus
typedef struct table_t table_t;
typedef struct ScreenCell ScreenCell;
#endif

#ifdef __cplusplus
extern "C" {
#endif 

static inline uint8_t align_to_color_palette(uint8_t color)
{
    return color & 0xF;
}

static inline uint8_t screen_cell_get_background_color(uint8_t x, uint8_t y)
{
    return (Screen.cursor[y][x] & 0xF000) >> 12;
}

static inline uint8_t screen_cell_get_foreground_color(uint8_t x, uint8_t y)
{
    return (Screen.cursor[y][x] & 0x0F00) >> 8;
}

static inline char screen_cell_get_character(uint8_t x, uint8_t y)
{
    return (char)Screen.cursor[y][x] & 0xFF;
}

void screen_cell_set(uint8_t x, uint8_t y, char character, uint8_t background_color, uint8_t foreground_color);
table_t* table_create(uint16_t x, uint16_t y, uint8_t number_of_rows, uint8_t row_size, uint8_t background_color, uint8_t foreground_color);
void table_insert(table_t* Table, uint8_t row_id, char* data, uint8_t background_color, uint8_t foreground_color);
void table_row_select(table_t* Table);
char* table_get_row_data(table_t* Table, uint8_t row_id, char* buffer);
void table_add_handler(table_t* Table, void(*handler)(char*));

#ifdef __cplusplus
}
#endif 