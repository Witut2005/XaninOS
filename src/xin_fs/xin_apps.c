
#include <xin_fs/xin.h>

__STATUS xin_create_file(char* entry_name)
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


__STATUS xin_create_folder(char* entry_name)
{

    int status = xin_create_directory(entry_name);

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

    return XANIN_OK;

}


__STATUS xin_move_entry(char* entry_name, char* new_name)
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

__STATUS xin_remove_directory(char* folder_name)
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


__STATUS xin_remove_entry(char* entry_name)
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