
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

// TERMINAL_APP

int xin_list_files_app(char **argv)
{

    char *options = (char *)null_region;
    char *name = (char *)NULL;

    for (int i = 1; argv[i][0] != '\0'; i++)
    {
        if (argv[i][0] == '-')
            options = argv[i];
        else
            name = argv[i];
    }

    bool show_hidden_files = bstrcmp(options, "-la");

    char path[XIN_MAX_PATH_LENGTH + 1] = {0};
    __xin_absolute_path_get(name, path, XIN_DIRECTORY);

    xprintf("Searching: %s\n", path);

    XinEntry *End = __xin_fs_entries_end_get();
    for (XinEntry *i = (XinEntry *)__xin_fs_entries_get(); (uint32_t)i < (uint32_t)End; i++)
    {

        if ((substr_find(i->path, "/.") && show_hidden_files == false)) // skip if hidden files should not be displayed
            continue;

        if (__xin_entry_pf_get(i->path) != NULL)
        // xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), __xin_entry_pf_get(i->path)->path);
        {
            if (bstrcmp(__xin_entry_pf_get(i->path)->path, path))
            {
                xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                xprintf("   ");
            }
        }

        // else
        // {
        //     if (is_using_abs_path)
        //     {
        //         // printed_text += strlen(i->path) + 2;

        //         if (bstrcmp(__xin_entry_pf_get(i->path)->path, path))
        //         {
        //             xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
        //             xprintf("  ");
        //         }
        //         // printed_text = printed_text + strlen("  ");
        //     }

        //     else
        //     {
        //         // if (bstrcmp(__xin_entry_pf_get(i->path)->path, __xin_path_get(path)))
        //         if (bstrcmp(__xin_entry_pf_get(i->path)->path, path))
        //         {
        //             xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
        //             xprintf("  ");
        //         }
        //     }
        // }
    }

    return XANIN_OK;
}