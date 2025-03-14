
#pragma once

#include <stdint.h>

struct table_t {
    uint16_t x;
    uint16_t y;
    uint16_t number_of_rows;

    uint16_t sites;

    uint16_t row_size;
    uint8_t background_color;
    uint8_t foreground_color;

    char*** row_data;
    uint8_t** row_background_color;
    uint8_t** row_foreground_color;

    uint8_t* DataObject;

    void (*handler)(char*, uint8_t*);
};

typedef void (*tui_table_handler)(char*, uint8_t*);

struct ScreenCell {
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

uint8_t align_to_color_palette(uint8_t color);
uint8_t screen_cell_get_background_color(uint8_t x, uint8_t y);
uint8_t screen_cell_get_foreground_color(uint8_t x, uint8_t y);
char screen_cell_get_character(uint8_t x, uint8_t y);

void screen_cell_set(uint8_t x, uint8_t y, char character, uint8_t background_color, uint8_t foreground_color);
table_t* table_create(uint16_t x, uint16_t y, uint8_t number_of_rows, uint8_t row_size, uint8_t background_color, uint8_t foreground_color, uint8_t number_of_sites, ...);
void table_insert(table_t* Table, uint8_t row_id, char* data, uint8_t background_color, uint8_t foreground_color, uint8_t page_id);
void table_row_select(table_t* Table);
char* table_get_row_data(table_t* Table, uint8_t row_id, uint8_t page_id);
void table_add_handler(table_t* Table, void (*handler)(char*, uint8_t*), uint8_t* DataObject);
void table_destroy(table_t* Table);
void tui_init(void);

#ifdef __cplusplus
}
#endif
