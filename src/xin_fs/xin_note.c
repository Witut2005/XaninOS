

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <terminal/interpreter.h>
#include <lib/exit_process.h>
#include <lib/string.h>




void note_input(void)
{

    uint8_t selected_character;
    uint8_t x_save, y_save;

    /*

    x_save = Screen.x;
    y_save = Screen.y;

    xprintf("%h",(cursor_set_position(0,27)));
    xprintf("         ");

    Screen.x = x_save;
    Screen.y = y_save;

    */

    if(KeyInfo.scan_code == F4_KEY || KeyInfo.scan_code == ESC)
    {
        app_exited = true;
    }

    else if(KeyInfo.is_bspc)
    {       
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

        if(!Screen.x)
        {
            Screen.y--;
            Screen.x = 79;
            return;
        }

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = '\0';
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | black) << 8));

        KeyInfo.is_bspc = false;
        return;
        letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
    
    }

    else if(KeyInfo.is_up)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        
        if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] >= VGA_TEXT_MEMORY)
            Screen.y--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

    }

    else if(KeyInfo.is_down)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        
        if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] <= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
            Screen.y++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

    }

    else if(KeyInfo.is_right)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        Screen.x++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));


        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }

    }

    else if(KeyInfo.is_left)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
    
        if(Screen.x == 0)
        {
            Screen.x = 80;
            Screen.y--;
        }


        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));


    }

    else if(KeyInfo.scan_code == ENTER)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        xprintf("\r\n");
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | white) << 8));
    }

    else if(KeyInfo.scan_code == TAB_KEY)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        Screen.x += 3;
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else
    {
        if(KeyInfo.character)
        {
            char character_saved_tmp = (char)Screen.cursor[Screen.y][Screen.x];
            xprintf("%c", getchar());
            letters_refresh_add(&Screen.cursor[Screen.y][Screen.x], character_saved_tmp);
        }
    }  


    /*
    x_save = Screen.x;
    y_save = Screen.y;

    xprintf("%h",(cursor_set_position(0,27)));
    xprintf("x%x y%x",x_save,y_save);

    Screen.x = x_save;
    Screen.y = y_save;

    
    else if(getscan() == DELETE_KEY)
    {
        Screen.cursor[Screen.y][Screen.x] = '\0';
        selected_character = '\0';
    }
    */

 
}

void xin_note(char* file_name)
{
    screen_clear();
    keyboard_handle = note_input;
    use_backspace = true;

    //xin_entry* xin_file = xin_find_entry(file_name);
    
    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file: %s\n", file_name);

        while(KeyInfo.scan_code != ENTER);

    }
        
    else
    {

        if(xin_file->os_specific == XIN_READ_ONLY)
        {
            xprintf("%zYOUR ARE EDITING READ-ONLY FILE. CHANGES WILL NOT BE SAVED.", set_output_color(red,white));
            while(KeyInfo.scan_code != ENTER);
        }

        //for(uint8_t* xin_pointer_table = (uint8_t*)(XIN_POINTER_TABLE + xin_entry->starting_sector); *xin_pointer_table != XIN_EOF; xin_pointer_table++)
        
        char* data_pointer = xin_file->starting_sector * SECTOR_SIZE;

        uint16_t* bruh_moment = VGA_TEXT_MEMORY;
        
        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
            bruh_moment[i] = (uint16_t) (data_pointer[i] + (((black << 4) | white) << 8));
        

        while(!app_exited);

        uint32_t file_data_counter = 0x0;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);

        if(xin_file->os_specific != XIN_READ_ONLY)
        {

        for(char* i = (char*)VGA_TEXT_MEMORY; 
            (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2, file_data_counter++)
                data_pointer[file_data_counter] = *i;

        xin_file->entry_size = file_data_counter;
        }

    }


    keyboard_handle = nullptr;
    exit_process();
}