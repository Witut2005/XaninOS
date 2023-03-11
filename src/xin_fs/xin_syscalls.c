
/*NO GETSCAN BROOOO*/

#include <xin_fs/xin.h>
#include <libc/stdlibx.h>
#include <libc/memory.h>
#include <libc/colors.h>
#include <libc/stdiox.h>
#include <libc/string.h>


__STATUS __sys_xin_file_create(char* entry_name)
{

    int status = xin_file_create(entry_name); 

    if(status == XIN_FILE_EXISTS)
    {
        // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
        // while(KeyInfo.scan_code != ENTER);
        return XIN_FILE_EXISTS;
    }

    else if(status == XANIN_ERROR)
    {
        // xprintf("%zFILE CREATE ERROR: %s\n", stderr, entry_name);
        // while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;
}


__STATUS __sys_xin_folder_create(char* entry_name)
{

    int status = xin_folder_create(entry_name);

    if(status == XIN_FILE_EXISTS)
    {
        // xprintf("%zDIRECTORY WITH THIS NAME EXISTS: %s\n", stderr, entry_name);
        // while(KeyInfo.scan_code != ENTER);
        return XIN_FILE_EXISTS;
    }

    else if(status == XANIN_ERROR)
    {
        // xprintf("%zDIRECTORY CREATION ERROR: %s\n", stderr, entry_name);
        // while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else if(status == XIN_BAD_FOLDER_NAME)
    {
        // xprintf("%zDIRECTORY CREATION ERROR (missing / and the end): %s\n", stderr, entry_name);
        // while(KeyInfo.scan_code != ENTER);
        return XIN_BAD_FOLDER_NAME;
    }

    return XANIN_OK;
}

__STATUS __sys_xin_entry_move(char* entry_name, char* new_name)
{

    if(xin_get_file_pf(new_name) == NULL)
    {
         return XIN_ENTRY_NOT_FOUND;
    }

    if(xin_find_entry(entry_name) == NULL)
    {
        return XIN_ENTRY_NOT_FOUND;
    }

    XinEntry* entry = xin_find_entry(entry_name);

    int i;
    if(new_name[0] == '/')
    {
        for(i = 0; new_name[i] != '\0'; i++)
            entry->path[i] = new_name[i];
    }
    else
    {
        for(i = 0; xin_get_current_path(new_name)[i] != '\0'; i++)
            entry->path[i] = xin_get_current_path(new_name)[i];

    }

    entry->path[i] = '\0';

    return XANIN_OK;

}


__STATUS __sys_xin_folder_remove(char* folder_name)
{
    __STATUS status = remove_directory(folder_name);

    if(status == XIN_ENTRY_NOT_FOUND)
        return XIN_ENTRY_NOT_FOUND;

    return XANIN_OK;

}


extern __STATUS __sys_xin_entry_remove(char* entry_name);


__STATUS __sys_xin_folder_change(const char* new_folder_name)
{
    XinEntry* folder_entry = xin_folder_change((char*)new_folder_name);

    if(folder_entry == NULL)
        return XANIN_ERROR;

    return XANIN_OK;
}


__STATUS __sys_xin_copy(char* file_name, char* new_file_name)
{

    XinEntry* entry = xin_find_entry(file_name);

    if(entry == NULL)
        return XIN_ENTRY_NOT_FOUND;

    int status = __sys_xin_file_create(new_file_name);

    if(status != XANIN_OK)
        return status;

    XinEntry* file = fopen(file_name, "r");
    XinEntry* file_created = fopen(new_file_name, "rw");

    char* entry_data = (char*)calloc(file->size);
    
    fread(file, entry_data, file->size);
    fwrite(file_created, entry_data, file->size);

    free(entry_data);
    fclose(&file);
    fclose(&file_created);

    return XANIN_OK;
}

__STATUS __sys_xin_link_remove(char* linkname)
{
    XinEntry* file = xin_find_entry(linkname); 

    if(file != NULL && file->type == XIN_LINK)
    {
        memset((uint8_t*)file, 0x0, sizeof(XinEntry));
        return XANIN_OK;
    }

    return XANIN_ERROR;


}

__STATUS __sys_xin_link_create(char* file_name, char* link_name)
{


    XinEntry* file = xin_find_entry(file_name); 

    if(file == NULL)
        return XIN_ENTRY_NOT_FOUND;

    XinEntry* link = xin_find_free_entry();
    memcpy((uint8_t*)link, (uint8_t*)file, sizeof(XinEntry));

    link->type = XIN_LINK;

    for(int i = 0; i < MAX_PATH; i++)
        link->path[i] = file->path[i];

    if(link_name[0] != '/')
        link_name = xin_get_current_path(link_name);

    for(int i = 0; i < MAX_PATH; i++)
        link->path[i] = link_name[i];

    return XANIN_OK;

}


__STATUS __sys_xin_list_files(char** argv)
{
    
    char* options = null_region;
    char* path = null_region;

    for(int i = 1; argv[i][0] != '\0'; i++)
    {
        if(argv[i][0] == '-')
            options = argv[i];
        else 
            path = argv[i];
    }


    XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; 

    int printed_text = 0;
    char* current_path = xin_get_current_path(path);

    if(xin_find_entry(path) == NULL && strlen(path) > 0)
        return XANIN_ERROR;

    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {
        
        if((substr_find(i->path, "/.") && !strcmp(options, "-la")) || (!i->path))
        {
            i++;
            continue; 
        }
        else if(!strlen(path))
        {
            if(xin_get_file_pf(i->path) != NULL)
            {
                if(strcmp(xin_get_file_pf(i->path)->path, xin_current_directory))
                {
                    xprintf("%z%s", set_output_color(black, i->type + 0x2), i);
                    xprintf("   ");
                }
            }
        }

        else 
        {
            if(path[0] == '/')
            {
                printed_text += strlen(i->path) + 2;

                if(printed_text >= 80)
                {
                    printed_text = 0;
                    xprintf("\n");
                    printed_text += strlen(i->path);
                }

                if(strcmp(xin_get_file_pf(i->path)->path, path))
                {
                    xprintf("%z%s", set_output_color(black, i->type + 0x2), i);
                    xprintf("  ");
                }
                printed_text = printed_text + strlen("  ");
        
            }
        
            else
            {
                if(strcmp(xin_get_file_pf(i->path)->path, xin_get_current_path(path)))
                {
                    xprintf("%z%s", set_output_color(black, i->type + 0x2), i);
                    xprintf("  ");
                }

            }

        }

        i++;
    }

    return XANIN_OK;

}
