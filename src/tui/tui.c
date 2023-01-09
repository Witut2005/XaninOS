
#include <tui/tui.h>
#include <libc/stdlibx.h>
#include <keyboard/scan_codes.h>

void screen_cell_set(uint8_t x, uint8_t y, char character, uint8_t background_color, uint8_t foreground_color)
{
    uint16_t* cursor = (uint16_t*)VGA_TEXT_MEMORY;

    background_color = align_to_color_palette(background_color);
    foreground_color = align_to_color_palette(foreground_color);

    cursor[x + y * VGA_WIDTH] = (uint16_t)(character | (((background_color << 4) | foreground_color) << 8));
}

table_t* table_create(uint16_t x, uint16_t y, uint8_t number_of_rows, uint8_t row_size, uint8_t background_color, uint8_t foreground_color)
{
    if((!number_of_rows) || (!row_size))
        return (table_t*)nullptr;

    table_t* tmp = (table_t*)calloc(sizeof(table_t));

    tmp->x = x;
    tmp->y = y;
    tmp->number_of_rows = number_of_rows;
    tmp->row_size = row_size;
    tmp->background_color = background_color;
    tmp->foreground_color = foreground_color;	

    for(int i = 0; i < number_of_rows+1; i++)
    {
        for(int j = x; j < x + row_size; j++)
            screen_cell_set(j, y+i*2, '-', background_color, foreground_color);
    }

    for(int i = 0; i < 3 + (2 * (number_of_rows-1)); i++)
    {
        screen_cell_set(x, y+i, '|', background_color, foreground_color);            
        screen_cell_set(x + row_size-1, y+i, '|', background_color, foreground_color);            
    }

    screen_cell_set(x, y, '|', background_color, foreground_color);
    screen_cell_set(x + row_size - 1, y, '|', background_color, foreground_color);

    screen_cell_set(x, y+number_of_rows*2, '|', background_color, foreground_color);
    screen_cell_set(x + row_size - 1, y+number_of_rows*2, '|', background_color, foreground_color);

    return tmp;
}


void table_insert(table_t* Table, uint8_t row_id, char* data, uint8_t background_color, uint8_t foreground_color)
{
    if(row_id > Table->number_of_rows)
        return;

    for(int i = 0; data[i] != '\0'; i++)
        screen_cell_set(Table->x+1+i, Table->y+1+(row_id*2), data[i], background_color, foreground_color);
}

char* table_get_row_data(table_t* Table, uint8_t row_id, char* buffer)
{
    uint8_t cursor_x = Table->x + 1; 
    uint8_t cursor_y = Table->y+1+(row_id*2);
    for(int i = 0; i < Table->row_size-2; i++)
        buffer[i] = screen_cell_get_character(cursor_x + i, cursor_y);
    return buffer;
}

void table_add_handler(table_t* Table, void(*handler)(char*))
{
    Table->handler = handler;
}

void table_row_select(table_t* Table)
{
    uint8_t cursor_x = Table->x; 
    uint8_t cursor_y = Table->y + 1; 
    uint8_t current_row = 0;

    ScreenCell SavedCell = {screen_cell_get_foreground_color(cursor_x, cursor_y), screen_cell_get_background_color(cursor_x, cursor_y), screen_cell_get_character(cursor_x, cursor_y)};
    xchar UserInput;

    char* buffer = (char*)calloc(Table->row_size);
    table_get_row_data(Table, current_row, buffer);

    for(int i = 1; i < Table->row_size-1; i++)
        screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);


    while(UserInput.scan_code != ENTER)
    {
        UserInput = inputg();

        if(UserInput.scan_code == ARROW_UP)
        {
            if(current_row - 1 < 0)
                continue;

            table_get_row_data(Table, current_row, buffer);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], black, white);

            cursor_y -= 2;

            table_get_row_data(Table, --current_row, buffer);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
            
        }

        else if(UserInput.scan_code == ARROW_DOWN)
        {
            if(current_row + 1 == Table->number_of_rows)
                continue;
            table_get_row_data(Table, current_row, buffer);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], black, white);

            cursor_y += 2;

            table_get_row_data(Table, ++current_row, buffer);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
        }

    }

    Table->handler(table_get_row_data(Table, current_row, buffer));
    KeyInfo.scan_code = 0;

}