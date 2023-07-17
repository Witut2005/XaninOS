
#include <stdarg.h>
#include <lib/tui/tui.h>
#include <lib/libc/stdlibx.h>
#include <sys/devices/keyboard/scan_codes.h>

// DO NOT USE STDIOX FUNCTIONS

void screen_cell_set(uint8_t x, uint8_t y, char character, uint8_t background_color, uint8_t foreground_color)
{
    uint16_t* cursor = (uint16_t*)VGA_TEXT_MEMORY;

    background_color = align_to_color_palette(background_color);
    foreground_color = align_to_color_palette(foreground_color);

    cursor[x + y * VGA_WIDTH] = (uint16_t)(character | (((background_color << 4) | foreground_color) << 8));
}

table_t* table_create(uint16_t x, uint16_t y, uint8_t number_of_rows, uint8_t row_size, uint8_t background_color, uint8_t foreground_color, uint8_t number_of_sites, ...)
{

    va_list args;
    va_start(args, number_of_sites);

    if((!number_of_rows) || (!row_size))
        return (table_t*)NULL;
    
    if(number_of_sites > 9)
        number_of_sites = 9;

    table_t* tmp = (table_t*)calloc(sizeof(table_t));

    tmp->x = x;
    tmp->y = y;
    tmp->number_of_rows = number_of_rows;
    tmp->row_size = row_size;
    tmp->background_color = background_color;
    tmp->foreground_color = foreground_color;	
    tmp->sites = number_of_sites;

    tmp->row_data = (char***)calloc(sizeof(char**) * number_of_sites);
    tmp->row_background_color = (uint8_t**)calloc(sizeof(uint8_t*) * number_of_sites);
    tmp->row_foreground_color = (uint8_t**)calloc(sizeof(uint8_t*) * number_of_sites);

    for(int i = 0; i < number_of_sites; i++)
    {
        tmp->row_background_color[i] = (uint8_t*)calloc(sizeof(uint8_t) * number_of_rows);
        tmp->row_foreground_color[i] = (uint8_t*)calloc(sizeof(uint8_t) * number_of_rows);
    }

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

    screen_cell_set(x, y, '0', background_color, foreground_color);
    screen_cell_set(x+1, y, '/', background_color, foreground_color);
    screen_cell_set(x+2, y, (char)(number_of_sites + '0' - 1), background_color, foreground_color);

    uint32_t row_id = 0;
    for(char* i = va_arg(args, char*); i != NULL; i = va_arg(args, char*), row_id++)
        table_insert(tmp, row_id, i, black, white, 0);

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

    Table->row_background_color[page_id][row_id] = background_color;
    Table->row_foreground_color[page_id][row_id] = foreground_color;

}

char* table_get_row_data(table_t* Table, uint8_t row_id, uint8_t page_id)
{
    return Table->row_data[page_id][row_id];
}

void table_add_handler(table_t* Table, void(*handler)(char*, uint8_t*), uint8_t* DataObject)
{
    Table->DataObject = DataObject;
    Table->handler = handler;
}

void table_row_select(table_t* Table)
{
    uint8_t cursor_x = Table->x; 
    uint8_t cursor_y = Table->y + 1; 
    uint8_t current_row = 0;
    uint8_t current_page = 0;

    xchar UserInput;
    char* buffer;

    buffer = table_get_row_data(Table, current_row, current_page);

    for(int i = 1; i < Table->row_size-1; i++)
        screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
    
    while(UserInput.scan_code != ENTER)
    {
        __sys_inputg(&UserInput);
        if(UserInput.scan_code == ARROW_RIGHT)
        {
            if(current_page < Table->sites-1)
                current_page++;
            else
                current_page = 0;

            current_row = 0;
            cursor_y = Table->y + 1;
            cursor_x = Table->x;

            for(int i = 0; i < Table->number_of_rows; i++)
            {            
                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, '\0', black, black);
            }

            for(int i = 0; i < Table->number_of_rows; i++)
            {            
                buffer = table_get_row_data(Table, i, current_page);

                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, buffer[j], Table->row_background_color[current_page][i], Table->row_foreground_color[current_page][i]);
            }

            buffer = table_get_row_data(Table, 0, current_page);
            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);

            screen_cell_set(Table->x, Table->y, (char)(current_page + '0'), Table->background_color, Table->foreground_color);
            screen_cell_set(Table->x+1, Table->y, '/', Table->background_color, Table->foreground_color);
            screen_cell_set(Table->x+2, Table->y, (char)(Table->sites + '0' - 1), Table->background_color, Table->foreground_color);
        }

        else if(UserInput.scan_code == ARROW_LEFT)
        {
            if(current_page > 0)
                current_page--;
            else
                current_page = Table->sites - 1;

            current_row = 0;
            cursor_y = Table->y + 1;
            cursor_x = Table->x;

            for(int i = 0; i < Table->number_of_rows; i++)
            {            
                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, '\0', black, black);
            }

            for(int i = 0; i < Table->number_of_rows; i++)
            {            
                buffer = table_get_row_data(Table, i, current_page);

                for(int j = 0; j < Table->row_size-2; j++)
                    screen_cell_set(cursor_x+1+j, cursor_y + i, buffer[j], Table->row_background_color[current_page][i], Table->row_foreground_color[current_page][i]);
            }

            buffer = table_get_row_data(Table, 0, current_page);
            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);

            screen_cell_set(Table->x, Table->y, (char)(current_page + '0'), Table->background_color, Table->foreground_color);
            screen_cell_set(Table->x+1, Table->y, '/', Table->background_color, Table->foreground_color);
            screen_cell_set(Table->x+2, Table->y, (char)(Table->sites + '0' - 1), Table->background_color, Table->foreground_color);
        }
        
        else if(UserInput.scan_code == ARROW_UP)
        {
            if(current_row - 1 < 0)
                continue;

            buffer = table_get_row_data(Table, current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], Table->row_background_color[current_page][current_row], Table->row_foreground_color[current_page][current_row]);

            cursor_y--;

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
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], Table->row_background_color[current_page][current_row], Table->row_foreground_color[current_page][current_row]);

            cursor_y++;
            buffer = table_get_row_data(Table, ++current_row, current_page);

            for(int i = 1; i < Table->row_size-1; i++)
                screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], white, black);
        }

    }
    
    for(int i = 1; i < Table->row_size-1; i++)
        screen_cell_set(cursor_x+i, cursor_y, buffer[i-1], Table->row_background_color[current_page][current_row], Table->row_foreground_color[current_page][current_row]);

    Table->handler(table_get_row_data(Table, current_row, current_page), Table->DataObject);
}

void table_destroy(table_t* Table)
{
    for(int i = Table->y; i < Table->y+1+(Table->number_of_rows * 2); i++)
    {
        for(int j = Table->x; j < Table->x + Table->row_size; j++)
            Screen.cursor[i][j] = 0;
    }
    
    for(int i = 0; i < Table->sites; i++)
    {
        for(int j = 0; j < Table->number_of_rows; j++)
            free(Table->row_data[i][j]);
        free(Table->row_data[i]);
    }

    for(int i = 0; i < Table->sites; i++)
    {
        free(Table->row_background_color[i]);
        free(Table->row_foreground_color[i]);
    }

    free(Table->row_data);
    free(Table->row_background_color);
    free(Table->row_foreground_color);

    free(Table);
}
