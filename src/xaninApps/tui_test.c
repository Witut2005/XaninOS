
#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>

char* app_current_folder;

extern int edit(char* file_name);

void hfs(char* omg)
{
    screen_clear();
    xprintf("option: %s\n", omg);
    while(1);
    // XinChildrenEntries* hoho = xin_get_children_entries(omg, false);
    // strcpy(app_current_folder, omg);
    // int status = __sys_xin_folder_change(omg);


    // if(status == XANIN_ERROR)
    // {
    //     edit(omg);
    // }

}

int my_tui_app(void)
{
    
    app_current_folder = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE);
    strcpy(app_current_folder, "/");

    while(KeyInfo.scan_code != F4_KEY) 
    {
        XinChildrenEntries* hoho = xin_get_children_entries(app_current_folder, false);
        
        table_t* fro = table_create(0,0, 10, 40, black, white, 1);

        // while(1);

        if(fro == nullptr)
        {
            screen_clear();
            xprintf("No folders");
            while(KeyInfo.scan_code != F4_KEY);
            break;
        }

        else
        {

            for(int i = 0; i < 5; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    if(i * 10 + j < hoho->how_many)
                        table_insert(fro , j, hoho->children[j + i * 10]->entry_path, black, hoho->children[j + i * 10]->entry_type + 2, i);
                }
            }



            // char* data = (char*)calloc(MAX_PATH);

            // table_add_handler(fro, hfs);
            // while(1);
            table_row_select(fro);
        }
    }

}
