
#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>

char* app_current_folder;

void hfs(char* omg)
{
    screen_clear();
    XinChildrenEntries* hoho = xin_get_children_entries("/config/");
    strcpy(app_current_folder, omg);
    int status = __sys_xin_folder_change(omg);

}

int my_tui_app(void)
{
    
    char* piwko = xin_get_entry_name("/config/nic.conf");
    xprintf("%s\n", piwko);

    while(inputg().scan_code != ENTER);
    
    app_current_folder = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE);
    strcpy(app_current_folder, "/");

    while(KeyInfo.scan_code != F4_KEY) 
    {
        XinChildrenEntries* hoho = xin_get_children_entries_type(app_current_folder, XIN_DIRECTORY);
        
        table_t* fro = table_create(0,0, hoho->how_many, 20, black, white);

        if(fro == nullptr)
        {
            screen_clear();
            xprintf("No folders");
            while(KeyInfo.scan_code != F4_KEY);
            break;
        }

        else
        {
            for(int i = 0; i < hoho->how_many; i++)
                table_insert(fro , i, hoho->children[i]->entry_path, black, hoho->children[i]->entry_type);

            char* data = (char*)calloc(MAX_PATH);

            table_add_handler(fro, hfs);
            table_row_select(fro);
        }
    }

}
