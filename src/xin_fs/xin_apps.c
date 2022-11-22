
#include <xin_fs/xin.h>

__STATUS __sys_xin_file_create(char* entry_name)
{

    int status = xin_create_file(entry_name); 

    if(status == XIN_FILE_EXISTS)
    {
        xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else if(status == XANIN_ERROR)
    {
        xprintf("%zFILE CREATE ERROR: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;


}


__STATUS __sys_xin_folder_create(char* entry_name)
{

    int status = xin_folder_create(entry_name);

    if(status == XIN_FILE_EXISTS)
    {
        xprintf("%zDIRECTORY WITH THIS NAME EXISTS: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else if(status == XANIN_ERROR)
    {
        xprintf("%zDIRECTORY CREATION ERROR: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else if(status == XIN_BAD_FOLDER_NAME)
    {
        xprintf("%zDIRECTORY CREATION ERROR (missing / and the end): %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;

}


__STATUS __sys_xin_entry_move(char* entry_name, char* new_name)
{
    
    __STATUS status = xin_move(entry_name, new_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        xprintf("%zENTRY NOT FOUND: %s\n", stderr, entry_name);    
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;

}

__STATUS __sys_xin_folder_remove(char* folder_name)
{
    __STATUS status = remove_directory(folder_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        xprintf("%zDIRECTORY NOT FOUND: %s\n", stderr, folder_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;

}


__STATUS __sys_xin_entry_remove(char* entry_name)
{
    __STATUS status = sys_xin_remove_entry(entry_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        xprintf("%zENTRY NOT FOUND: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    return XANIN_OK;

}


__STATUS __sys_xin_folder_change(const char* new_folder_name)
{
    xin_entry* folder_entry = xin_folder_change(new_folder_name);
    {
        if(folder_entry == nullptr)
        {
            xprintf("%zCANT CHANGE DIRECTORY (SYSCALL EXIT STATUS = 3)\n", stderr);
            while(KeyInfo.scan_code != ENTER);
            return XANIN_ERROR;
        }
        
        else 
            return XANIN_OK;
    }
}


__STATUS __sys_xin_copy(char* file_name, char* new_file_name)
{

    xin_entry* entry = xin_find_entry(file_name);

    if(entry == nullptr)
    {
        xprintf("%zNO SUCH FILE: %s\n", stderr, file_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    xin_create_file(new_file_name);

    xin_entry* new_entry = xin_find_entry(new_file_name);

    int counter = 0; 
    char* entry_data = entry->starting_sector * SECTOR_SIZE;
    char* new_entry_data = new_entry->starting_sector * SECTOR_SIZE;
    
    for(; counter <= SECTOR_SIZE * 0x10; counter++)
    {
        new_entry_data[counter] = entry_data[counter];
    }
    return XANIN_OK;
}

__STATUS __sys_xin_link_create(char* file_name, char* link_name)
{


    uint8_t* file = (uint8_t*)xin_find_entry(file_name); 

    if(file == nullptr)
    {
        xprintf("%zNO SUCH FILE\n", stderr);
        while(getscan() != ENTER);
        return XANIN_ERROR;
    } 

    uint8_t* link = (uint8_t*)xin_find_free_entry();

    for(int i = 0; i < sizeof(xin_entry); i++)
        link[i] = file[i];

    if(link_name[0] != '/')
        link_name = xin_get_current_path(link_name);

    for(int i = 0; i < 40; i++)
        link[i] = link_name[i];

    return XANIN_OK;

}


int __sys_xin_list_files(char** argv)
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


    xin_entry* i = (xin_entry*)XIN_ENTRY_TABLE; 

    int printed_text = 0;
    char* current_path = xin_get_current_path(path);

    while(i->entry_path[0] != '\0')
    {
        if(strcmp(options, "-s"))
        {
            if(printed_text + strlen(i->entry_path) > VGA_WIDTH)
            {
                xprintf("\n");
                printed_text = 0;
            }
            xprintf("%s   ", i->entry_path);
            printed_text += strlen(i->entry_path) + 4;
        }

        else 
        {

            if(substr_find(i->entry_path, "/.") && !strcmp(options, "-la"))
            {
                i++;
                continue;
            }

            if(strlen(path) == 0)
            {
                printed_text += strlen(i->entry_path) + 2;

                if(printed_text >= 80)
                {
                    printed_text = 0;
                    xprintf("\n");
                    printed_text += strlen(i->entry_path);
                }

                if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_current_directory))
                {
                    xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                    xprintf("  ");
                }
                printed_text = printed_text + strlen("  ");
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
                    printed_text += strlen(i->entry_path) + 2;

                    if(printed_text >= 80)
                    {
                        printed_text = 0;
                        xprintf("\n");
                        printed_text += strlen(i->entry_path);
                    }

                    if(strcmp(xin_get_file_pf(i->entry_path)->entry_path, xin_get_current_path(path)))
                    {
                        xprintf("%z%s", set_output_color(black, i->entry_type + 0x2), i);
                        xprintf("  ");
                    }
                    printed_text = printed_text + strlen("  ");

                }

            }
        }

        i++;
    }

    while(KeyInfo.scan_code != ENTER);
    return XANIN_OK;

}
