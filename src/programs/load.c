
#pragma once

#include <lib/ascii/ascii.h>
#include <sys/terminal/backend/backend.h>

int load(char* address_string)
{

    char* data_pointer = (char*)strtoi(address_string, HEXADECIMAL); 

    xtb_disable_flushing();

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        char tmp[9] = {0x0};
        xprintf("0x%X:", data_pointer + i * 16);
        
        for(int k = 0; k < 8 - strlen(int_to_hex_str(data_pointer + i * 16, tmp)); k++) //better address aligment (visually)
            puts(" ");
        
        for(int j = 0; j < 16; j++)
            xprintf(" %mX", data_pointer[ (16 * i) + j]);

        xprintf(" %z| ", OUTPUT_COLOR_SET(black, green));

        for(int j = 0; j < 16; j++)
        {
            if(data_pointer[ (16 * i) + j] < ASCII_SPACE)
                puts(" ");
            // else
            //     xprintf("%c", data_pointer[ (16 * i) + j]);
        }
        puts("\n");
    }

    xtb_enable_flushing();
    xtb_flush_all(vty_get());

    return XANIN_OK;
}
