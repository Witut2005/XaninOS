

#pragma once

int load(char* address_string)
{

    screen_clear();


    uint32_t address = strtoi(address_string, HEXADECIMAL);

    char* data_pointer = (char*)(address);


    for(int i = 0; i < 28; i++)
    {
        
        char tmp[9] = {0x0};
        xprintf("0x%X:", address + i * 16);
        
        for(int k = 0; k < 8 - strlen(int_to_hex_str(address + i * 16, tmp)); k++) //better address aligment (visually)
            xprintf(" ");
        
        for(int j = 0; j < 16; j++)
            xprintf(" %mX", data_pointer[ (16 * i) + j]);

        xprintf(" %z| ", OUTPUT_COLOR_SET(black, green));

        for(int j = 0; j < 16; j++)
            putchar(data_pointer[ (16 * i) + j]);

        xprintf("\n");
    }

	while(inputg().scan_code != ENTER);
    return XANIN_OK;


}
