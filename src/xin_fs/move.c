
#include <xin_fs/xin.c>

void xin_move(char* entry_name, char* new_name)
{

    if(xin_get_file_pf(new_name) == nullptr)
    {
         xprintf("%zNO SUCH DIRECTORY", set_output_color(red,white));
         while(getscan() != ENTER);
         return;
    }

    if(xin_find_entry(entry_name) == nullptr)
    {
        xprintf("%zNO SUCH ENTRY", set_output_color(red, white));
        while(getscan() != ENTER);
        return;
    }

    xin_entry* entry = xin_find_entry(entry_name);

    int i;
    for(i = 0; new_name[i] != '\0'; i++)
        entry->entry_path[i] = new_name[i];

    entry->entry_path[i] = '\0';
    

}

