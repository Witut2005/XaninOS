
#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>

static char* app_current_folder;
static char** app_parent_folder;
static uint32_t app_parent_folder_counter;

extern int edit(char* file_name);

#define TUI_TEST_SITES 2

void hfs(char* omg)
{

    if(!strlen(omg))
        return;

    else if(strcmp(omg, "..") && strcmp(app_current_folder, "/"))
        exit();

    else if(strcmp(omg, ".."))
    {
        if(app_parent_folder_counter)
            app_parent_folder_counter--;
        strcpy(app_current_folder, app_parent_folder[app_parent_folder_counter]);
        free(app_parent_folder[app_parent_folder_counter]);
    }

    else
    {
        app_parent_folder = (char**)realloc(app_parent_folder, sizeof(char*) * (app_parent_folder_counter + 1));
        app_parent_folder[app_parent_folder_counter] = (char*)calloc(80);
        strcpy(app_parent_folder[app_parent_folder_counter], app_current_folder);
        app_parent_folder_counter++;
        strcpy(app_current_folder, omg);
    }


    // XinChildrenEntries* hoho = xin_get_children_entries(omg, false);

    int status = __sys_xin_folder_change(app_current_folder);

    if(status == XANIN_ERROR)
        edit(omg);

}

int my_tui_app(void)
{
    app_current_folder = nullptr;
    app_parent_folder = nullptr;
    app_parent_folder_counter = 0;

    app_current_folder = (char*)calloc(80);
    strcpy(app_current_folder, "/");

    app_parent_folder = (char**)calloc(sizeof(char*));

    while(KeyInfo.scan_code != F4_KEY) 
    {
        XinChildrenEntries* hoho = xin_get_children_entries(app_current_folder, false);
        table_t* fro = table_create(0,0, 10, 40, black, white, TUI_TEST_SITES);

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
            for(int i = 0; i < TUI_TEST_SITES; i++)
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
        free(hoho);
        free(fro);
    }

    for(int i = 0; i<app_parent_folder_counter; i++)
        free(app_parent_folder[i]);

    free(app_parent_folder);
    free(app_current_folder);
    exit();

    return XANIN_OK;

}
