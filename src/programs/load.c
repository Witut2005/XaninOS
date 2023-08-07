
#pragma once
#include <lib/ascii/ascii.h>

int load(char* address_string)
{

    // stdio_mode_set(STDIO_MODE_CANVAS);
    // screen_clear();

    char* data_pointer = (char*)strtoi(address_string, HEXADECIMAL); 

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        char tmp[9] = {0x0};
        xprintf("0x%X:", data_pointer + i * 16);
        
        for(int k = 0; k < 8 - strlen(int_to_hex_str(data_pointer + i * 16, tmp)); k++) //better address aligment (visually)
            xprintf(" ");
        
        for(int j = 0; j < 16; j++)
            xprintf(" %mX", data_pointer[ (16 * i) + j]);

        xprintf(" %z| ", OUTPUT_COLOR_SET(black, green));

        for(int j = 0; j < 16; j++)
        {
            
            if(data_pointer[ (16 * i) + j] < ASCII_SPACE)
                putchar(' ');
            else
                putchar(data_pointer[ (16 * i) + j]);
        }
        xtb_flush(vty_get());

        xprintf("\n");
    }
	while(inputg().scan_code != ENTER);
    return XANIN_OK;

}
