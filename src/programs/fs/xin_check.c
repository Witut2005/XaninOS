
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

int xin_check(char *path)
{

    XinEntry *ParentEntry = __xin_entry_pf_get(path);

    if (ParentEntry != NULL)
        xprintf("parent folder: %s\n", ParentEntry->path);
    else
        xprintf("no parent folder :((\n");

    XANIN_DEBUG_RETURN(XANIN_OK);
    // xprintf("checking %s\n", path);

    // xprintf("Entries table 0x%x\n", __xin_fs_entries_get());
    // xprintf("Entries table end 0x%x\n", __xin_fs_entries_get() + (SECTOR_SIZE * __xin_fs_entries_size_get()));
    // XinEntry *Entry = __xin_find_entry(path);

    // if (Entry != NULL)
    //     xprintf("%zEXISTS", OUTPUT_COLOR_SET(black, green));
    // else
    //     xprintf("%zBRUH", OUTPUT_COLOR_SET(black, red));
}