
/*NO GETSCAN BROOOO*/

#include <xin_fs/xin.h>

__STATUS __sys_xin_file_create(char* entry_name)
{

    int status = xin_create_file(entry_name); 

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


__STATUS __sys_XinEntry_move(char* entry_name, char* new_name)
{
    
    __STATUS status = xin_move(entry_name, new_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        // xprintf("%zENTRY NOT FOUND: %s\n", stderr, entry_name);    
        // while(KeyInfo.scan_code != ENTER);
        return XIN_ENTRY_NOT_FOUND;
    }

    return XANIN_OK;

}

__STATUS __sys_xin_folder_remove(char* folder_name)
{
    __STATUS status = remove_directory(folder_name);

    if(status == XIN_ENTRY_NOT_FOUND)
        return XIN_ENTRY_NOT_FOUND;

    return XANIN_OK;

}


__STATUS __sys_XinEntry_remove(char* entry_name)
{
    __STATUS status = sys_xin_remove_entry(entry_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        return XIN_ENTRY_NOT_FOUND;
    }

    return XANIN_OK;

}


__STATUS __sys_xin_folder_change(const char* new_folder_name)
{
    XinEntry* folder_entry = xin_folder_change(new_folder_name);

    if(folder_entry == nullptr)
    {
        return XANIN_ERROR;
    }



    return XANIN_OK;
}


__STATUS __sys_xin_copy(char* file_name, char* new_file_name)
{

    XinEntry* entry = xin_find_entry(file_name);

    if(entry == nullptr)
        return XIN_ENTRY_NOT_FOUND;

    int status = __sys_xin_file_create(new_file_name);

    if(status != XANIN_OK)
        return status;

    XinEntry* file = fopen(file_name, "r");
    XinEntry* file_created = fopen(new_file_name, "rw");

    char* entry_data = (char*)calloc(SECTOR_SIZE);
    
    for(int counter = 0; counter < file->entry_size / SECTOR_SIZE; counter++)
    {
        fread(file, entry_data, SECTOR_SIZE);
        fwrite(file_created, entry_data, SECTOR_SIZE);
    }

    free(entry_data);
    fclose(&file);
    fclose(&file_created);

    return XANIN_OK;
}

__STATUS __sys_xin_link_remove(char* linkname)
{
    XinEntry* file = xin_find_entry(linkname); 

    if(file != nullptr && file->entry_type == XIN_LINK)
    {
        memset((uint8_t*)file, 0x0, sizeof(XinEntry));
        return XANIN_OK;
    }

    else
    {
        return XANIN_ERROR;
    }


}

__STATUS __sys_xin_link_create(char* file_name, char* link_name)
{


    XinEntry* file = xin_find_entry(file_name); 

    if(file == nullptr)
        return XIN_ENTRY_NOT_FOUND;

    XinEntry* link = xin_find_free_entry();
    memcpy((uint8_t*)link, (uint8_t*)file, sizeof(XinEntry));

    link->entry_type = XIN_LINK;

    for(int i = 0; i < MAX_PATH; i++)
        link->entry_path[i] = file->entry_path[i];

    if(link_name[0] != '/')
        link_name = xin_get_current_path(link_name);

    for(int i = 0; i < MAX_PATH; i++)
        link->entry_path[i] = link_name[i];

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

    if(xin_find_entry(path) == nullptr && strlen(path) > 0)
    {
        xprintf("%zNO SUCH DIRECTORY\n", stderr);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_OK;
    }

    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {
        
        if((substr_find(i->entry_path, "/.") && !strcmp(options, "-la")) || (!i->entry_path))
        {
            i++;
            continue; 
        }
        else if(!strlen(path))
        {
            if(xin_get_file_pf(i->entry_path) != nullptr)
            {
                if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_current_directory))
                {
                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("   ");
                }
            }
        }

        else 
        {
            if(path[0] == '/')
            {
                printed_text += strlen(i->entry_path) + 2;

                if(printed_text >= 80)
                {
                    printed_text = 0;
                    xprintf("\n");
                    printed_text += strlen(i->entry_path);
                }

                if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, path))
                {
                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("  ");
                }
                printed_text = printed_text + strlen("  ");
        
            }
        
            else
            {
                if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_get_current_path(path)))
                {
                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("  ");
                }

            }

        }

        i++;
    }

    return XANIN_OK;

}
