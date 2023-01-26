#include <xin_fs/xin.h>

int xin_info(char *entry_name)
{

    screen_clear();

    XinEntry* file = fopen(entry_name, "r");

    if (file != nullptr)
    {
        xprintf("\nentry path: %s\n", file->entry_path);
        xprintf("creation date: %y\n", file->creation_date);
        xprintf("creation time: %t\n", file->creation_time);
        xprintf("FileInformationBlock location: 0x%x\n", file->FileInfo);
        xprintf("modification_date: %y\n", file->modification_date);
        xprintf("modification_time: %t\n", file->modification_time);
        xprintf("entry_permissions: 0x%x\n", file->entry_permissions);
        xprintf("entry_size: 0x%x\n", file->entry_size);
        xprintf("starting_sector: 0x%x\n", file->starting_sector);
    }

    else
    {
        xprintf("%zNO SUCH FILE\n", set_output_color(red, white));
        while (inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    fclose(&file);

    while (inputg().scan_code != ENTER);
    return XANIN_OK;
}

