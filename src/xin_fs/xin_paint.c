

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <lib/exit_process.h>
#include <lib/string.h>

#define VGA_SCREEN_RESOLUTION 4480


uint8_t current_color;

void paint_input(void)
{

    /*

    uint8_t x_save, y_save;

    x_save = Screen.x;
    y_save = Screen.y;

    xprintf("%h",(cursor_set_position(0,27)));
    xprintf("         ");

    Screen.x = x_save;
    Screen.y = y_save;

    */

    uint16_t selected_cell;

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] < VGA_TEXT_MEMORY)
    {
        Screen.x = 0x0;
        Screen.y = 0x0;
        selected_cell = '\0';
    }

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] >= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
    {
        Screen.x = 79;
        Screen.y = 27;
        selected_cell = '\0';
    }

    if(KeyInfo.scan_code == F4_KEY)
    {
        app_exited = true;
    }

    else if(KeyInfo.is_up)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] >= VGA_TEXT_MEMORY)
            Screen.y--;
   

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);        
    }

    else if(KeyInfo.is_down)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] <= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
            Screen.y++;

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);   
    }

    else if(KeyInfo.is_right)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;


        Screen.x++; 

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
         
        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }
    
    }

    else if(KeyInfo.is_left)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if(Screen.x == 0)
        {
            Screen.x = 80;
            Screen.y--;
        }

        
        Screen.x--;

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    else if(KeyInfo.scan_code == F3_KEY)
    {
        selected_cell = 0x0;
        screen_clear();
    }

    else
    {
        switch(KeyInfo.character)
        {
            case '0': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((black << 4) | black) << 8)); current_color = black; break;
            case '1': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((blue << 4) | black) << 8)); current_color = blue; break;
            case '2': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((green << 4) | black) << 8)); current_color = green;break;
            case '3': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((cyan << 4) | black) << 8)); current_color = cyan; break;
            case '4': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((red << 4) | black) << 8)); current_color = red; break;
            case '5': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((magenta << 4) | black) << 8)); current_color = magenta; break;
            case '6': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((brown << 4) | black) << 8)); current_color = brown; break;
            case '7': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lgray << 4) | black) << 8)); current_color = lgray; break;
            case '8': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((dgray << 4) | black) << 8)); current_color = dgray; break;
            case '9': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lblue << 4) | black) << 8)); current_color = lblue; break;
            case 'a': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lgreen << 4) | black) << 8)); current_color = lgreen; break;
            case 'b': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lcyan << 4) | black) << 8)); current_color = lcyan; break;
            case 'c': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lred << 4) | black) << 8)); current_color = lred; break;
            case 'd': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lmagenta << 4) | black) << 8)); current_color = lmagenta; break;
            case 'e': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((yellow << 4) | black) << 8)); current_color = yellow; break;
            case 'f': Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((white << 4) | black) << 8)); current_color = white; break;
        }

        if(KeyInfo.character)
            selected_cell = (uint16_t) (' ' + (((current_color << 4) | black) << 8));
    }

 

}


void xin_paint(char* file_name)
{
    screen_clear();

    keyboard_handle = paint_input;


    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return;
    }

    else
    {
                
    
        if(xin_file->os_specific == XIN_READ_ONLY)
        {
            xprintf("%zYOUR ARE EDITING READ-ONLY FILE. CHANGES WILL NOT BE SAVED.", set_output_color(red,white));
            while(KeyInfo.scan_code != ENTER);
        }

        char* data_pointer = xin_file->starting_sector * SECTOR_SIZE;
        
        uint16_t* bruh_moment = VGA_TEXT_MEMORY;


        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
        {
            bruh_moment[i] = (uint16_t)(' ' | ((data_pointer[i+1]) << 8));
        }
       
        Screen.x = 0x0;
        Screen.y = 0x0;

        while(!app_exited);

        uint32_t file_data_counter = 0x1;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);

        if(xin_file->os_specific != XIN_READ_ONLY)
        {
            for(char* i = (char*)(VGA_TEXT_MEMORY + 1); 
                    (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2, file_data_counter++)
                    data_pointer[file_data_counter] = *i;

            xin_file->entry_size = file_data_counter;
        }

    }

    keyboard_handle = nullptr;
    return;

}