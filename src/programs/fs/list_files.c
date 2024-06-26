
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

// TERMINAL_APP

int xin_list_files_app(char** argv)
{

    char* options = (char*)null_region;
    char* name = (char*)NULL;

    for (int i = 1; argv[i][0] != '\0'; i++)
    {
        if (argv[i][0] == '-')
            options = argv[i];
        else
            name = argv[i];
    }

    bool show_hidden_files = bstrcmp(options, "-la");

    char path[XIN_MAX_PATH_LENGTH + 1] = { 0 };
    __xin_absolute_path_get(name, path);

    xprintf("Searching: %s\n", path);

    XinEntry* End = __xin_fs_entries_end_get();
    for (XinEntry* i = (XinEntry*)__xin_fs_entries_get(); i < End; i++)
    {
        if ((substr_find(i->path, "/.") && show_hidden_files == false)) // skip if hidden files should not be displayed
            continue;

        if (strlen(i->path) && __xin_parent_folder_entry_get(i->path) != NULL)
        {
            if (bstrcmp(__xin_parent_folder_entry_get(i->path)->path, path))
            {
                xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                xprintf("   ");
            }
        }
    }

    return XANIN_OK;
}
