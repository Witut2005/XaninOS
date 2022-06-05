#include <xin_fs/xin.h>

void xin_entry_info(char *entry_name)
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
        xprintf("file address: 0x%x\n", file->starting_sector * SECTOR_SIZE);
    }

    else
    {
        xprintf("%zNO SUCH FILE\n", set_output_color(red, white));
    }

    while (KeyInfo.scan_code != ENTER);
    return;
}

