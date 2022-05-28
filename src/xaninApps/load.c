

#pragma once

#include <lib/stdiox.h>

void load(char* address_string)
{

    screen_clear();


    uint32_t address = strtoi(address_string, 16);
    char* data_pointer = (char*)(address);

    char tmp[9] = {0};

    for(int i = 0; i < 28; i++)
    {
        
        xprintf("0x%X:", address + i * 16);

        for(int k = 0; k < 8 - strlen(int_to_hex_str(address + i * 16, tmp)); k++)
            xprintf(" ");
        

        for(int j = 0; j < 16; j++)
        {
            xprintf(" %mX", data_pointer[ (16 * i) + j]);
        }

        xprintf(" %z| ", set_output_color(black, green));

        for(int j = 0; j < 16; j++)
        {
            xprintf("%c", data_pointer[ (16 * i) + j]);
        }

        

        xprintf("\n");
    }

	while(KeyInfo.scan_code != ENTER);

	

}
