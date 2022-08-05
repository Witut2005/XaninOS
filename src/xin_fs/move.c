
#include <xin_fs/xin.h>

__STATUS xin_move(char* entry_name, char* new_name)
{

    if(xin_get_file_pf(new_name) == nullptr)
    {
        //  xprintf("%zNO SUCH DIRECTORY", set_output_color(red,white));
        //  while(getscan() != ENTER);
         return XIN_ENTRY_NOT_FOUND;
    }

    if(xin_find_entry(entry_name) == nullptr)
    {
        // xprintf("%zNO SUCH ENTRY", set_output_color(red, white));
        // while(getscan() != ENTER);
        return XIN_ENTRY_NOT_FOUND;
    }

    xin_entry* entry = xin_find_entry(entry_name);

    int i;
    for(i = 0; new_name[i] != '\0'; i++)
        entry->entry_path[i] = new_name[i];

    entry->entry_path[i] = '\0';

    return XANIN_OK;

}

