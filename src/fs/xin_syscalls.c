
/*NO GETSCAN BROOOO*/

#include <fs/xin.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>
#include <lib/libc/colors.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

__STATUS __sys_xin_entry_move(char *entry_name, char *new_name)
{

    if (xin_get_file_pf(new_name) == NULL)
    {
        return XIN_ENTRY_NOT_FOUND;
    }

    if (xin_find_entry(entry_name) == NULL)
    {
        return XIN_ENTRY_NOT_FOUND;
    }

    XinEntry *entry = xin_find_entry(entry_name);

    int i;
    if (new_name[0] == '/')
    {
        for (i = 0; new_name[i] != '\0'; i++)
            entry->path[i] = new_name[i];
    }
    else
    {
        for (i = 0; xin_get_current_path(new_name)[i] != '\0'; i++)
            entry->path[i] = xin_get_current_path(new_name)[i];
    }

    entry->path[i] = '\0';

    return XANIN_OK;
}
