
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

    XinEntry *i = (XinEntry *)XIN_ENTRY_TABLE;

    int printed_text = 0;

    if (__xin_find_entry(path) == NULL && strlen(path) > 0)
        return XANIN_ERROR;

    while ((uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50))
    {

        if ((substr_find(i->path, "/.") && !bstrcmp(options, "-la"))) // || (i->path != NULL))
        {
            i++;
            continue;
        }

        else if (!strlen(path))
        {
            if (__xin_get_file_pf(i->path) != NULL)
            {
                char buf[XIN_MAX_PATH_LENGTH];
                if (bstrcmp(__xin_get_file_pf(i->path)->path, __xin_current_directory_get(buf)))
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

                if (bstrcmp(__xin_get_file_pf(i->path)->path, path))
                {
                    xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                    xprintf("  ");
                }
                printed_text = printed_text + strlen("  ");
            }

            else
            {
                if (bstrcmp(__xin_get_file_pf(i->path)->path, __xin_path_get(path)))
                {
                    xprintf("%z%s", OUTPUT_COLOR_SET(black, i->type + 0x2), i);
                    xprintf("  ");
                }
            }
        }

        i++;
    }

    return XANIN_OK;
}