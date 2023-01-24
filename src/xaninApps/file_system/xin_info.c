#include <xin_fs/xin.h>

int xin_entry_info(char *entry_name)
{

    xin_entry* file = fopen(entry_name, "r");

    if (file != nullptr)
    {
        xprintf("\nentry path: %s\n", file->entry_path);
        xprintf("creation date: %y\n", file->creation_date);
        xprintf("creation time: %t\n", file->creation_time);
        xprintf("file_information_block location: 0x%x\n", file->file_info);
        xprintf("modification_date: %y\n", file->modification_date);
        xprintf("modification_time: %t\n", file->modification_time);
        xprintf("entry_permissions: 0x%x\n", file->entry_permissions);
        xprintf("entry_size: 0x%x\n", file->entry_size);
        xprintf("starting_sector: 0x%x\n", file->starting_sector);

        fseek(file, 10);
        fwrite(file, "12345", 5);

        xprintf("memory block[0] state (begin): %d\n", xin_get_memory_block_state(file)[0].begin);
        xprintf("memory block[0] state (end): %d\n", xin_get_memory_block_state(file)[0].end);

        xprintf("memory block[1] state (begin): %d\n", xin_get_memory_block_state(file)[1].begin);
        xprintf("memory block[1] state (end): %d\n", xin_get_memory_block_state(file)[1].end);
    }

    else
    {
        xprintf("%zNO SUCH FILE\n", set_output_color(red, white));
        while (KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    fclose(&file);

    while (KeyInfo.scan_code != ENTER);
    return XANIN_OK;
}

