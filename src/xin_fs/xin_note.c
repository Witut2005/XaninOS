

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <terminal/interpreter.h>
#include <lib/exit_process.h>
#include <lib/string.h>

#define VGA_SCREEN_RESOLUTION 4480

void note_input(void)
{

    uint8_t selected_character;

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] < VGA_TEXT_MEMORY)
    {
        Screen.y++;
    }

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] > VGA_TEXT_MEMORY + SCREEN_RESOLUTION)
    {
        Screen.y--;
    }

    if(KeyInfo.is_up)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | (((black << 4) | white) << 8));

        Screen.y--;

        /*
            if((char)Screen.cursor[Screen.y][Screen.x] != 0x20 && (char)Screen.cursor[Screen.y][Screen.x] != '\0')
                Screen.x--;
        */

        selected_character = (char)Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    else if(KeyInfo.is_down)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | (((black << 4) | white) << 8));

        Screen.y++;

        /*
        if((char)Screen.cursor[Screen.y][Screen.x] != 0x20 && (char)Screen.cursor[Screen.y][Screen.x] != '\0')
            Screen.x--;
        */


        selected_character = (char)Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    else if(KeyInfo.is_right)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | (((black << 4) | white) << 8));
        Screen.x++;    

        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }

        selected_character = (char)Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | (((black << 4) | white) << 8));
    }

    else if(KeyInfo.is_left)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)(selected_character | (((black << 4) | white) << 8));

        if(Screen.x == 0)
        {
            Screen.x = 79;
            Screen.y--;
        }

        Screen.x--;

        selected_character = (char)Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    else
    {
        xprintf("%c", getchar());
        if(getchar())
            selected_character = (char)Screen.cursor[Screen.y][Screen.x];
    }  

    /*
    else if(getscan() == DELETE_KEY)
    {
        Screen.cursor[Screen.y][Screen.x] = '\0';
        selected_character = '\0';
    }
    */

 
}

void xin_note(char* file_name)
{
    clearScr();
    keyboard_handle = note_input;


    xin_entry* xin_file = xin_find_entry(file_name);
    
    if(xin_file == nullptr)
    {
        xprintf("%zno such file or directory %s\n",set_output_color(red,white),program_parameters);
        keyboard_handle = nullptr;
        while(1)
        {
            if((getscan() == F4_KEY) || (getscan() == ENTER))
            {
                exit_process();
            }
        }   
    
    }
        
    else
    {
    
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zyou cant edit directory\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(keyboard_scan_code != F4_KEY);
            exit_process();
        }


        //for(uint8_t* xin_pointer_table = (uint8_t*)(XIN_POINTER_TABLE + xin_entry->starting_sector); *xin_pointer_table != XIN_EOF; xin_pointer_table++)
        
        char* data_pointer = xin_file->starting_sector * SECTOR_SIZE;

        
        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
        {
            xprintf("%c",data_pointer[i]);
        }

        while(getscan() != F4_KEY && getscan() != ESC);

        uint32_t file_data_counter = 0x0;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);

        for(char* i = (char*)VGA_TEXT_MEMORY; 
                (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2)
                if(*i == '\0')
                    *i = 0x20;

        for(char* i = (char*)VGA_TEXT_MEMORY; 
                (uint32_t)i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i+=2, file_data_counter++)
                data_pointer[file_data_counter] = *i;

        xin_file->entry_size = file_data_counter;

    }


    keyboard_handle = nullptr;
    exit_process();
}