#pragma once

#include <libc/stdiox.h>
#include <xin_fs/xin.h>

void list_files(char* path)
{

    xin_entry* i = (xin_entry*)XIN_ENTRY_TABLE; 

    int printed_text = 0;

    while(*(char*)i != '\0')
    {


        // xprintf("%s\n   ", i->entry_path);

        if((uint32_t)i >= XIN_ENTRY_TABLE + (SECTOR_SIZE * 8))
            return;

        if(strlen(path) == 0)
        {
            if(xin_find_entry(i->entry_path) != nullptr)
            {
                printed_text += strlen(i->entry_path);

                if(printed_text >= 70)
                {
                    printed_text = 0;
                    xprintf("\n");
                }

                if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_current_directory))
                {
                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("  ");
                }
            }
        }


        else
        {

            if(xin_find_entry(i->entry_path) != nullptr)
            {
                if(strncmp(i->entry_path, path, strlen(path)))
                {
                    printed_text += strlen(i->entry_path);

                    if(printed_text >= 70)
                    {
                        printed_text = 0;
                        xprintf("\n");
                    }

                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("  ");

                }
            }
        }


        i++;
    }

    while(KeyInfo.scan_code != ENTER);

}
