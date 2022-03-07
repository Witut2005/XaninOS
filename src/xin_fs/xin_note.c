

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <lib/exit_process.h>
#include <lib/string.h>

void xin_note(char* file_name)
{
    no_enter = true;
    clearScr();
    arrows_navigate = true;
    //cursor_show = true;


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


        //for(uint8_t* xin_pointer_table = (uint8_t*)(XIN_POINTER_TABLE + xin_entry->starting_sector); *xin_pointer_table != XIN_EOF; xin_pointer_table++)
        
        char* data_pointer;

        {
            for(char* i = (char*)(xin_file->starting_sector * SECTOR_SIZE);  i < (char*)((xin_file->starting_sector * SECTOR_SIZE) + SECTOR_SIZE); i++)
            {
                xprintf("%c", *i);
            }
        }


        cursor = (uint16_t*)(VGA_TEXT_MEMORY);
        while(keyboard_scan_code != F4_KEY);

        uint32_t file_data_counter = 0x0;

        data_pointer= xin_file->starting_sector * SECTOR_SIZE;
        
        for(char* i = (char*)VGA_TEXT_MEMORY; 
                (uint32_t)i < VGA_TEXT_MEMORY + (512 * 2); i+=2, file_data_counter++)
                data_pointer[file_data_counter] = *i;

        xin_file->entry_size = file_data_counter;


    }

    
    exit_process();
}