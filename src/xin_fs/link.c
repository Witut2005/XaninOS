
#include <xin_fs/xin.h>

void xin_link_create(char* file_name, char* link_name)
{


    uint8_t* file = (uint8_t*)xin_find_entry(file_name); 

    if(file == nullptr)
    {
        xprintf("%zNO SUCH FILE\n", stderr);
        while(getscan() != ENTER);
        return;
    }

    uint8_t* link = (uint8_t*)xin_find_free_entry();

    for(int i = 0; i < sizeof(xin_entry); i++)
        link[i] = file[i];

    if(link_name[0] != '/')
        link_name = xin_get_current_path(link_name);

    for(int i = 0; i < 40; i++)
        link[i] = link_name[i];

}
