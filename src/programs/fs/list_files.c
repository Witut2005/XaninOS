
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

// TERMINAL_APP

int xin_list_files_app(char **argv)
{

    char *options = (char *)null_region;
    char *path = (char *)null_region;

    for (int i = 1; argv[i][0] != '\0'; i++)
    {
        if (argv[i][0] == '-')
            options = argv[i];
        else
            path = argv[i];
    }

    int printed_text = 0;

    if (__xin_find_entry(path) == NULL && strlen(path) > 0)
        return XANIN_ERROR;

    XinFileSystemData XinFsData = __xin_fs_data_get();
    XinEntry *End = __xin_fs_entries_end_get();

    for (XinEntry *i = (XinEntry *)__xin_fs_entries_get(); (uint32_t)i < (uint32_t)End; i++)
    {

        if ((substr_find(i->path, "/.") && !bstrcmp(options, "-la"))) // || (i->path != NULL))
        {
            i++;
            continue;
        }

        else if (!strlen(path))
        {
            if (__xin_entry_pf_get(i->path) != NULL)
            {
                char buf[XIN_MAX_PATH_LENGTH];
                if (bstrcmp(__xin_entry_pf_get(i->path)->path, __xin_current_directory_get(buf)))
                {
                    xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                    xprintf("   ");
                }
            }
        }

        else
        {
            if (path[0] == '/')
            {
                printed_text += strlen(i->path) + 2;

                if (bstrcmp(__xin_entry_pf_get(i->path)->path, path))
                {
                    xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                    xprintf("  ");
                }
                printed_text = printed_text + strlen("  ");
            }

            else
            {
                // if (bstrcmp(__xin_entry_pf_get(i->path)->path, __xin_path_get(path)))
                if (bstrcmp(__xin_entry_pf_get(i->path)->path, path))
                {
                    xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                    xprintf("  ");
                }
            }
        }
    }

    return XANIN_OK;
}