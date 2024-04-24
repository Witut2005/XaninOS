
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

int xin_check(char* path)
{

    char buf[XIN_MAX_PATH_LENGTH + 1] = { '\0' };

    // xprintf("xin pf extern: %s\n", __xin_path_pf_extern("/f/o/../../", pfbuf));
    xprintf("xin abs extern: %s\n", __xin_absolute_path_get(path, buf));
    // xprintf("xin path pf extern: %s\n", __xin_path_pf_extern(path, buf));

    // XinEntry *ParentEntry = __xin_parent_folder_entry_get(path);

    // if (ParentEntry != NULL)
    //     xprintf("parent folder: %s\n", ParentEntry->path);
    // else
    //     xprintf("no parent folder :((\n");

    // ParentEntry = __xin_entry_pf_extern(path);

    // if (ParentEntry != NULL)
    //     xprintf("parent folder: %s\n", ParentEntry->path);
    // else
    //     xprintf("no parent folder :((\n");

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
