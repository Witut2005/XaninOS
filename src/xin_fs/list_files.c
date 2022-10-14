#pragma once

#include <libc/stdiox.h>
#include <xin_fs/xin.h>

void list_files(char* path)
{

    xin_entry* i = (xin_entry*)XIN_ENTRY_TABLE; 

    int printed_text = 0;

    while(*(char*)i != '\0')
    {


        if(strlen(path) == 0)
        {
            printed_text += strlen(i->entry_path);

            if(printed_text >= 80)
            {
                printed_text = 0;
                xprintf("\n");
                printed_text += strlen(i->entry_path);
            }

            if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_current_directory))
            {
                xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                xprintf("  ");
            }
            printed_text = printed_text + strlen("  ");
        }


        else
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
            
            printed_text = printed_text + strlen("  ");

        }


        i++;
    }

    while(KeyInfo.scan_code != ENTER);

}