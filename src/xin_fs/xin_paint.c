

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <lib/exit_process.h>
#include <lib/string.h>

#define VGA_SCREEN_RESOLUTION 4480


uint8_t current_color;

void paint_input(void)
{

    char selected_color;

    if(KeyInfo.is_up)
    {
        Screen.y--;
    }

    else if(KeyInfo.is_down)
    {
        Screen.y++;
    }

    else if(KeyInfo.is_right)
    {
        Screen.x++;    
    }

    else if(KeyInfo.is_left)
    {
        Screen.x--;
    }

        
}


void xin_paint(char* file_name)
{
    clearScr();

    keyboard_handle = paint_input;


    xin_entry* xin_file = xin_find_entry(file_name);
    
    if(xin_file == nullptr)
    {
        xprintf("%zno such file or directory %s\n",set_output_color(red,white),program_parameters);
        
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
            xprintf("%zyou cant edit directory\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(keyboard_scan_code != ENTER);
            exit_process();
        }


        char* data_pointer;
        
        for(char* i = (xin_file->starting_sector * SECTOR_SIZE);  (uint32_t)i < (xin_file->starting_sector + VGA_SCREEN_RESOLUTION); i += 2)
        {
            xprintf("%z%c", set_output_color( (*(i+1) >> 4), *(i+1)), 0x20);
        }
        

        while(keyboard_scan_code != F4_KEY)
        {

            switch(getchar())
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
                default: break;
            }



        }


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