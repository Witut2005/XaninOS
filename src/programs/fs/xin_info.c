
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

//TERMINAL_APP

int xin_info(char *entry_name)
{

    XinEntry* file = xin_find_entry(entry_name);

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
    }

    else
    {
        xprintf("%zNO SUCH FILE\n", OUTPUT_COLOR_SET(red, white));
        return XANIN_ERROR;
    }

    return XANIN_OK;
}
