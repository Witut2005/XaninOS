

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <lib/exit_process.h>
#include <lib/string.h>

#define VGA_SCREEN_RESOLUTION 4480


uint8_t current_color;

void paint_input(void)
{


    uint8_t x_save, y_save;

    x_save = Screen.x;
    y_save = Screen.y;

    xprintf("%h",(cursor_set_position(0,27)));
    xprintf("         ");

    Screen.x = x_save;
    Screen.y = y_save;

    uint16_t selected_cell;

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] < VGA_TEXT_MEMORY)
    {
        Screen.x = 0x0;
        Screen.y = 0x0;
        selected_cell = '\0';
    }

    if(KeyInfo.is_up)
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

    x_save = Screen.x;
    y_save = Screen.y;

    xprintf("%h",(cursor_set_position(0,27)));
    xprintf("x%x y%x",x_save,y_save);

    Screen.x = x_save;
    Screen.y = y_save;        
}


void xin_paint(char* file_name)
{
    screen_clear();

    keyboard_handle = paint_input;


    xin_entry* xin_file = xin_find_entry(file_name);
    
    if(xin_file == nullptr)
    {
    
        xprintf("%zNo such file or directory %s\n",set_output_color(red,white),program_parameters);
        
        while(1)
        {
            if(keyboard_scan_code == ENTER)
            {
                arrows_navigate = false;
                exit_process();
            }
        }   
    }
        
    else
    {
                
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zYou can't edit directory\n",set_output_color(red,white));
            xprintf("%zUse F4 key to exit\n",set_output_color(red,white));
            while(keyboard_scan_code != ENTER);
            exit_process();
        }

        char* data_pointer;
        
        for(char* i = (xin_file->starting_sector * SECTOR_SIZE);  (uint32_t)i < ((xin_file->starting_sector + 0x9 ) * SECTOR_SIZE); i += 2)
        {
            xprintf("%z%c", set_output_color( (*(i+1) >> 4), *(i+1)), ' ');
        }
       
        Screen.x = 0x0;
        Screen.y = 0x0;

        while(KeyInfo.scan_code != F4_KEY);

        uint32_t file_data_counter = 0x0;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);


        for(char* i = (char*)(VGA_TEXT_MEMORY); 
                (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++, file_data_counter++)
                data_pointer[file_data_counter] = *i;

        xin_file->entry_size = file_data_counter;


    }

    keyboard_handle = nullptr;
    exit_process();

}