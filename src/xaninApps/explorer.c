

#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>
#include <libc/process.h>
#include <libc/data_structures.h>


extern int edit(char* file_name);
extern int xin_info(char *entry_name);

struct ExplorerInfo
{
    char* current_folder;
    bool exit_tui_app;
    char* selected_file;
    table_t* MainTable;
};
typedef struct ExplorerInfo ExplorerInfo;

#define TUI_TEST_SITES 2

void handle_selected_option(char* option, ExplorerInfo* AppInfo)
{
    if(strcmp(option, "remove"))
    {
        xprintf("selected file: %s\n", AppInfo->selected_file);
        __sys_xin_entry_remove(AppInfo->selected_file);
    }

    else if(strcmp(option, "rename"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 25;
        xprintf("New file name: ");
        xscanf("%s", new_name);
        __sys_xin_entry_move(AppInfo->selected_file, xin_get_current_path(new_name));

        free(new_name);
    }

    else if(strcmp(option, "link"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 25;

        xprintf("where do you want to create a link entry: ");
        xscanf("%s", new_name);

        xprintf("\ngiven name: %s\n", new_name);
        // while(1);

        int status;
        if(new_name[0] != '/')
            status = __sys_xin_link_create(AppInfo->selected_file, xin_get_current_path(new_name));
        else
            status = __sys_xin_link_create(AppInfo->selected_file, new_name);
        
        free(new_name);
    }

    else if(strcmp(option, "copy"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 23;

        xprintf("where do you want to create a copy entry: ");
        xscanf("%s", new_name);

        xprintf("selected file: %s\n", AppInfo->selected_file);
        xprintf("new file: %s\n", new_name[0] != '/' ? xin_get_current_path(new_name) : new_name);

        __sys_xin_copy(AppInfo->selected_file, new_name);
        
        free(new_name);
    }


    else if(strcmp(option, "edit"))
    {
        edit(AppInfo->selected_file);
    }

    else if(strcmp(option, "create"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 25;
        xprintf("New file name: ");
        xscanf("%s", new_name);
        __sys_xin_file_create(new_name);
        xprintf("new file: %s\n", xin_get_current_path(new_name));
        free(new_name);
    }

    else if(strcmp(option, "info"))
    {
        xin_info(AppInfo->selected_file);
    }


}

void hfs(char* omg, ExplorerInfo* AppInfo)
{

    
    if(strcmp(omg, "exit"))
    {
        AppInfo->exit_tui_app = true;
        return;
    }

    else if(strcmp(omg, ""))
    {
        table_t* fro = table_create(0, 15, 1, 20, black, white, 1);
        table_insert(fro, 0, "create", black, white, 0);
        table_add_handler(fro, (tui_table_handler)handle_selected_option, (address_t)AppInfo);
        table_row_select(fro);

        table_destroy(fro);
        return;
    }

    else if(strcmp(omg, "..") && strcmp(AppInfo->current_folder, "/"))
        AppInfo->exit_tui_app = true;


    else if(strcmp(omg, ".."))
    {
        xin_get_current_directory(AppInfo->current_folder);
        __sys_xin_folder_change(xin_get_file_pf(AppInfo->current_folder)->path);
        return;
    }

    int status = __sys_xin_folder_change(omg);

    if(status == XANIN_ERROR)
    {
        table_t* fro = table_create(0, 15, 6, 20, black, white, 1);
        table_insert(fro , 0, "remove", black, white, 0);
        table_insert(fro , 1, "rename", black, white, 0);
        table_insert(fro , 2, "link", black, white, 0);
        table_insert(fro , 3, "copy", black, white, 0);
        table_insert(fro , 4, "info", black, white, 0);
        table_insert(fro , 5, "edit", black, white, 0);

        AppInfo->selected_file = omg;
        table_add_handler(fro, (tui_table_handler)handle_selected_option, (address_t)AppInfo);
        table_row_select(fro);

        table_destroy(fro);
    }

}

void explorer_app_deconstructor(void)
{
    // table_destroy();
    // free(explorer_main_table);
}

int explorer(char* parent_folder)
{

    ExplorerInfo AppInfo = {NULL};

    app_process_register(explorer_app_deconstructor, 2, AppInfo.current_folder, AppInfo.selected_file);

    char* initial_folder = (char*)calloc(MAX_PATH);
    xin_get_current_directory(initial_folder);

    AppInfo.current_folder = (char*)calloc(MAX_PATH);

    if(!strlen(parent_folder)) // no path given
        xin_get_current_directory(AppInfo.current_folder);

    else if(parent_folder[strlen(parent_folder) - 1] != '/') // no '/' at the end
        parent_folder[strlen(parent_folder)] = '/';

    else if(xin_find_entry(parent_folder) != NULL) // ok
        strcpy(AppInfo.current_folder, xin_find_entry(parent_folder)->path) ;

    else  // no such directory 
    { 
        xprintf("%zNo such directory: %s\n", stderr, parent_folder);
        strcpy(AppInfo.current_folder, "/");
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    __sys_xin_folder_change(AppInfo.current_folder);

    while(KeyInfo.scan_code != F4_KEY) 
    {
        screen_clear();
        xin_get_current_directory(AppInfo.current_folder);
        xprintf("CURRENT DIRECTORY: %s\n", AppInfo.current_folder);
        XinChildrenEntries* hoho = xin_get_children_entries(AppInfo.current_folder, false);
        AppInfo.MainTable = table_create(0,1, 10, 80, black, white, TUI_TEST_SITES);

        if(AppInfo.MainTable== NULL)
        {
            screen_clear();
            xprintf("No folders");
            while(inputg().scan_code != F4_KEY);
            break;
        }

        else
        {
            table_add_handler(AppInfo.MainTable, (tui_table_handler)hfs, (address_t)&AppInfo);

            for(int i = 0; i < TUI_TEST_SITES; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    if(i * 10 + j < hoho->how_many)
                        table_insert(AppInfo.MainTable, j, hoho->children[j + i * 10]->path, black, hoho->children[j + i * 10]->type + 2, i);
                         
                    else
                    {
                        table_insert(AppInfo.MainTable, j, "..", black, white, i);
                        table_insert(AppInfo.MainTable, j+1, "exit", black, red, i);
                        break;
                    }
                }
            }

            table_row_select(AppInfo.MainTable);
        }

        table_destroy(AppInfo.MainTable);
        free(hoho);
        free(AppInfo.MainTable);

        if(AppInfo.exit_tui_app)
            break;
    }


    free(AppInfo.current_folder);

    __sys_xin_folder_change(initial_folder);
    free(initial_folder);

    return XANIN_OK;

}
