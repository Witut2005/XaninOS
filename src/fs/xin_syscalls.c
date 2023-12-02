
/*NO GETSCAN BROOOO*/

#include <fs/xin.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>
#include <lib/libc/colors.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

__STATUS __sys_xin_file_create(char *entry_name)
{
    return xin_file_create(entry_name);
}

__STATUS __sys_xin_folder_create(char *entry_name)
{
    return xin_folder_create(entry_name);
}

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

__STATUS __sys_xin_folder_remove(char *folder_name)
{
    return remove_directory(folder_name);
}

extern __STATUS __sys_xin_entry_remove(char *entry_name);

__STATUS __sys_xin_folder_change(const char *new_folder_name)
{
    XinEntry *folder_entry = xin_folder_change((char *)new_folder_name);

    if (folder_entry == NULL)
        return XANIN_ERROR;

    return XANIN_OK;
}

__STATUS __sys_xin_copy(char *file_name, char *new_file_name)
{

    XinEntry *entry = xin_find_entry(file_name);

    if (entry == NULL)
        return XIN_ENTRY_NOT_FOUND;

    int status = __sys_xin_file_create(new_file_name);

    if (status != XANIN_OK)
        return status;

    XinEntry *file = fopen(file_name, "r");
    XinEntry *file_created = fopen(new_file_name, "rw");

    char *entry_data = (char *)calloc(file->size);

    fread(file, entry_data, file->size);
    fwrite(file_created, entry_data, file->size);

    free(entry_data);
    fclose(&file);
    fclose(&file_created);

    return XANIN_OK;
}

__STATUS __sys_xin_link_remove(char *linkname)
{
    XinEntry *file = xin_find_entry(linkname);

    if (file != NULL && file->type == XIN_LINK)
    {
        memset((uint8_t *)file, 0x0, SIZE_OF(XinEntry));
        return XANIN_OK;
    }

    return XANIN_ERROR;
}

__STATUS __sys_xin_link_create(char *file_name, char *link_name)
{
    XinEntry *file = xin_find_entry(file_name);

    if (file == NULL)
        return XIN_ENTRY_NOT_FOUND;

    XinEntry *link = xin_find_free_entry();
    memcpy((uint8_t *)link, (uint8_t *)file, SIZE_OF(XinEntry));

    link->type = XIN_HARD_LINK;

    for (int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        link->path[i] = file->path[i];

    if (link_name[0] != '/')
        link_name = xin_get_current_path(link_name);

    for (int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        link->path[i] = link_name[i];

    return XANIN_OK;
}
