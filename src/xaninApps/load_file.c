

#pragma once

int load_file(char* file_name, char* options)
{

    uint32_t offset = 0;

    if(strcmp(options, "-offset"))
        offset = strtoi(argv[3], 16);

    screen_clear();

    XinEntry* File = fopen(file_name, "rw");

    uint8_t* file_data = (uint8_t*)calloc(File->entry_size);
    fread(File, file_data, File->entry_size);

    uint32_t address = (uint32_t)file_data + offset;
    char* data_pointer = (char*)file_data + offset;

    for(int i = 0; i < 28; i++)
    {
        
        char tmp[9] = {0x0};
        xprintf("0x%X:", address + i * 16);
        
        for(int k = 0; k < 8 - strlen(int_to_hex_str(address + i * 16, tmp)); k++) //better address aligment (visually)
            xprintf(" ");
        
        for(int j = 0; j < 16; j++)
            xprintf(" %mX", data_pointer[ (16 * i) + j]);

        xprintf(" %z| ", set_output_color(black, green));

        for(int j = 0; j < 16; j++)
            putchar(data_pointer[ (16 * i) + j]);

        xprintf("\n");
    }

	while(inputg().scan_code != ENTER);
    return XANIN_OK;


}
