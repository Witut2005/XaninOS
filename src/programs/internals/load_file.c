

#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>

extern char *argv[5]; // USE HERE SYSCALL

// TERMINAL_APP

int load_file(char *file_name, char *options)
{

    uint32_t offset = 0;

    if (bstrcmp(options, "-offset"))
        offset = strtoi(argv[3], HEXADECIMAL);

    screen_clear();

    XinEntry *File = fopen(file_name, "r");

    uint8_t *file_data = (uint8_t *)calloc(File->size);
    fread(File, file_data, File->size);

    uint32_t address = (uint32_t)file_data + offset;
    char *data_pointer = (char *)file_data + offset;

    for (int i = 0; i < 28; i++)
    {

        char tmp[9] = {0x0};
        xprintf("0x%X:", address + i * 16);

        for (int k = 0; k < 8 - strlen(int_to_hex_str(address + i * 16, tmp)); k++) // better address aligment (visually)
            xprintf(" ");

        for (int j = 0; j < 16; j++)
            xprintf(" %mX", data_pointer[(16 * i) + j]);

        xprintf(" %z| ", OUTPUT_COLOR_SET(black, green));

        for (int j = 0; j < 16; j++)
        {

            if (data_pointer[(16 * i) + j] <= ASCII_SPACE)
                putchar(' ');
            else
                putchar(data_pointer[(16 * i) + j]);
        }

        xprintf("\n");
    }

    fclose(&File);

    return XANIN_OK;
}
