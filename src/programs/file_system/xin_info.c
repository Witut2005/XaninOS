#include <fs/xin.h>

int xin_info(char *entry_name)
{

    screen_clear();

    XinEntry* file = fopen(entry_name, "rw");

    if (file != NULL)
    {
        xprintf("\nentry path: %s\n", file->path);
        xprintf("creation date: %y\n", file->creation_date);
        xprintf("creation time: %t\n", file->creation_time);
        xprintf("FileInformationBlock location: 0x%x\n", file->FileInfo);
        xprintf("modification_date: %y\n", file->modification_date);
        xprintf("modification_time: %t\n", file->modification_time);
        xprintf("permissions: 0x%x\n", file->permissions);
        xprintf("size: %d\n", file->size);
        xprintf("first_sector: 0x%x\n", file->first_sector);
        // fwrite(file, "nicho", 5);
    }

    else
    {
        xprintf("%zNO SUCH FILE\n", OUTPUT_COLOR_SET(red, white));
        while (inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }
    
    fwrite(file, "nicho", 5);
    fclose(&file);


    while (inputg().scan_code != ENTER);
    return XANIN_OK;
}
