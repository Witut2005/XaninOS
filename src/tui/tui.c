
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

table_t* table_create(uint16_t x, uint16_t y, uint8_t number_of_rows, uint8_t row_size, uint8_t background_color, uint8_t foreground_color, uint8_t number_of_sites)
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
    tmp->sites = number_of_sites;

    tmp->row_data = (char***)calloc(sizeof(char**) * number_of_sites);

    for(int i = 0; i < number_of_sites; i++)
    {
        tmp->row_data[i] = (char**)calloc(sizeof(char**));

        for(int j = 0; j < number_of_rows; j++)
            tmp->row_data[i][j] = (char*)calloc(80);
    }

    for(int i = 0; i < row_size; i++)
        screen_cell_set(i, y, '-', background_color, foreground_color);

    for(int i = 0; i < row_size; i++)
        screen_cell_set(i, y+number_of_rows+1, '-', background_color, foreground_color);

    for(int i = 0; i < number_of_rows+2; i++)
    {
        if(tmp->y + i + 1 >= VGA_HEIGHT)
            break;

        screen_cell_set(x, y+i, '|', background_color, foreground_color);            
        screen_cell_set(x + row_size-1, y+i, '|', background_color, foreground_color);            
    }

    return tmp;
}

void table_insert(table_t* Table, uint8_t row_id, char* data, uint8_t background_color, uint8_t foreground_color, uint8_t page_id)
{
    if(row_id > Table->number_of_rows)
        return;

    for(int i = 0; data[i] != '\0'; i++)
    {
        if(!page_id)
            screen_cell_set(Table->x+1+i, Table->y+1+row_id, data[i], background_color, foreground_color);
        Table->row_data[page_id][row_id][i] = data[i];
    }
    // xprintf("given data: %s\n", Table->row_data);
}

char* table_get_row_data(table_t* Table, uint8_t row_id, uint8_t page_id)
{
    return Table->row_data[page_id][row_id];
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
    uint8_t current_page = 0;

    ScreenCell SavedCell = {screen_cell_get_foreground_color(Table->x+1, Table->y+1), screen_cell_get_background_color(Table->x+1, Table->y+1), screen_cell_get_character(Table->x+1, Table->y+1)};
    xchar UserInput;
    char* buffer;

    buffer = table_get_row_data(Table, current_row, current_page);

    for(int i = 1; i < Table->row_size-1; i++)
        screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);

    while(UserInput.scan_code != ENTER)
    {
        UserInput = inputg();

        if(UserInput.scan_code == ARROW_RIGHT)
        {
            if(current_page + 1 < Table->sites)
                current_page++;

            current_row = 0;
            cursor_y = Table->y + 1;
            cursor_x = Table->x;

            for(int i = 0; i < Table->number_of_rows; i++)
            {            
                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, '\0', SavedCell.background_color, SavedCell.foreground_color);
            }

            for(int i = 0; i < Table->number_of_rows; i++)
            {            

                buffer = table_get_row_data(Table, i, current_page);

                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, buffer[j], SavedCell.background_color, SavedCell.foreground_color);
            }
        }

        else if(UserInput.scan_code == ARROW_LEFT)
        {
            if(current_page > 0)
                current_page--;

            current_row = 0;

            buffer = table_get_row_data(Table, current_row, current_page);

            for(int i = 1; i < Table->row_size-2; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], SavedCell.background_color, SavedCell.foreground_color);

            cursor_y = Table->y + 1;
            cursor_x = Table->x;

            SavedCell.foreground_color = screen_cell_get_foreground_color(Table->x+1, cursor_y);
            SavedCell.background_color = screen_cell_get_background_color(Table->x+1, cursor_y);

            buffer = table_get_row_data(Table, --current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
        }
        
        else if(UserInput.scan_code == ARROW_UP)
        {
            if(current_row - 1 < 0)
                continue;

            buffer = table_get_row_data(Table, current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], SavedCell.background_color, SavedCell.foreground_color);

            cursor_y--;

            SavedCell.foreground_color = screen_cell_get_foreground_color(Table->x+1, cursor_y);
            SavedCell.background_color = screen_cell_get_background_color(Table->x+1, cursor_y);

            buffer = table_get_row_data(Table, --current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
            
        }

        else if(UserInput.scan_code == ARROW_DOWN)
        {
            if(current_row + 1 == Table->number_of_rows)
                continue;

            buffer = table_get_row_data(Table, current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], SavedCell.background_color, SavedCell.foreground_color);

            cursor_y++;

            SavedCell.foreground_color = screen_cell_get_foreground_color(Table->x+1, cursor_y);
            SavedCell.background_color = screen_cell_get_background_color(Table->x+1, cursor_y);

            buffer = table_get_row_data(Table, ++current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
        }

    }
    Table->handler(table_get_row_data(Table, current_row, current_page));
    KeyInfo.scan_code = 0;
}

void table_destroy(table_t* Table)
{
    for(int i = Table->y; i < Table->y+1+(Table->number_of_rows * 2); i++)
    {
        for(int j = Table->x; j < Table->x + Table->row_size; j++)
            Screen.cursor[i][j] = 0;
    }
    free(Table);
}
