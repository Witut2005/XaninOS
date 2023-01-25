
#include <xin_fs/xin.h>

__STATUS xin_move(char* entry_name, char* new_name)
{

    if(xin_get_file_pf(new_name) == nullptr)
    {
         return XIN_ENTRY_NOT_FOUND;
    }

    if(xin_find_entry(entry_name) == nullptr)
    {
        return XIN_ENTRY_NOT_FOUND;
    }

    XinEntry* entry = xin_find_entry(entry_name);

    int i;
    if(new_name[0] == '/')
    {
        for(i = 0; new_name[i] != '\0'; i++)
            entry->entry_path[i] = new_name[i];
    }
    else
    {
        for(i = 0; xin_get_current_path(new_name)[i] != '\0'; i++)
            entry->entry_path[i] = xin_get_current_path(new_name)[i];

    }

    entry->entry_path[i] = '\0';

    return XANIN_OK;

}

