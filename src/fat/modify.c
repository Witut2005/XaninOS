


#include <fat/fat_driver.h>
#include <fat/find_file.c>
#include <lib/string.h>
#include <lib/stdiox.h>

void modify(char* file_name)
{


    union fat_file
    {
    
        char fat_file_id[12];

        struct
        {
            char file_name[8];
            char file_extension[3];
   
        };
    }file;


    uint8_t file_name_length_counter = 0x0;

    while(file_name[file_name_length_counter] != '.')
    {
        if(file_name_length_counter > 8)
            xprintf("%zFILE NAME CANT BE MORE THAN 8 CHARACTERS LONG", 
                                        set_output_color(red,white));
        file_name_length_counter++;
    }

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
    {

        if(i < file_name_length_counter)
        {
            if(file_name[i] < 0x20)
                file.file_name[i] = 0x20;
            
            else 
                file.file_name[i] = file_name[i];
        }
    
        else
        {
            file.file_name[i] = 0x20;
        }
    
    }

    if(find_character('.', file_name) == nullptr)
    {
        xprintf("%zINCORRECT FILE EXTENSION\n",set_output_color(red,white));
    }

    int extension_counter = 0x0;
    for(char* i = find_character('.', file_name) + 1; i < find_character('.', file_name) + 4; i++, extension_counter++)
    {

        if(*i < 0x20)
            file.file_extension[extension_counter] = 0x20;

        else 
            file.file_extension[extension_counter] = *i;
    }

    xprintf("your file: %s\n", file.fat_file_id);
   
    if(find_file(file.fat_file_id) != nullptr)
    {
        xprintf("%zTHIS FILE EXIST\n", set_output_color(green, white));
    }

    else 
    {
        xprintf("%zNO SUCH FILE\n", set_output_color(red, white));
    }

    keyboard_scan_code = 0x0;

    while(keyboard_scan_code != ENTER);

    app_exited = true;

}
