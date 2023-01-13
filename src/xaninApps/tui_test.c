
#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>

static char* app_current_folder;
static char* app_parent_folder;

extern int edit(char* file_name);

void hfs(char* omg)
{
    XinChildrenEntries* hoho = xin_get_children_entries(omg, false);

    if(!strcmp(omg, ".."))
    {
        strcpy(app_current_folder, omg);
    }
    else
    {
        strcpy(omg, app_parent_folder);
        strcpy(app_current_folder, app_parent_folder);
    }

    int status = __sys_xin_folder_change(omg);

    if(status == XANIN_ERROR)
        edit(omg);

}

int my_tui_app(void)
{
    
    app_current_folder = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE);
    strcpy(app_current_folder, "/");
    strcpy(app_parent_folder, "/");

    while(KeyInfo.scan_code != F4_KEY) 
    {
        XinChildrenEntries* hoho = xin_get_children_entries(app_current_folder, false);
        
        table_t* fro = table_create(0,0, 10, 40, black, white, 5);

        if(fro == nullptr)
        {
            screen_clear();
            xprintf("No folders");
            while(KeyInfo.scan_code != F4_KEY);
            break;
        }

        else
        {
            table_add_handler(fro, hfs);
            for(int i = 0; i < 5; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    if(i * 10 + j < hoho->how_many)
                        table_insert(fro , j, hoho->children[j + i * 10]->entry_path, black, hoho->children[j + i * 10]->entry_type + 2, i);
                    else
                    {
                        table_insert(fro , j, "..", black, white, i);
                        break;
                    }
                }
            }
            table_row_select(fro);
        }
        table_destroy(fro);
    }

}
