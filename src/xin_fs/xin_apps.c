
#include <xin_fs/xin.h>

__STATUS __sys_xin_file_create(char* entry_name)
{

    int status = create_file(entry_name); 

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