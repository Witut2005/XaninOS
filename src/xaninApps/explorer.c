
#include <tui/tui.h>
#include <libc/colors.h>
#include <xin_fs/xin.h>

static char* app_current_folder;
static char** app_parent_folder;
static uint32_t app_parent_folder_counter;
static bool exit_tui_test_app;
static char* selected_file;

extern int edit(char* file_name);

#define TUI_TEST_SITES 2

void handle_selected_option(char* option)
{

        // table_insert(fro , 0, "remove", black, white, 0);
        // table_insert(fro , 1, "rename", black, white, 0);
        // table_insert(fro , 2, "link", black, white, 0);
        // table_insert(fro , 3, "copy", black, white, 0);
        // table_insert(fro , 4, "info", black, white, 0);
        // table_insert(fro , 5, "edit", black, white, 0);

    if(strcmp(option, "remove"))
    {
        xprintf("selected file: %s\n", selected_file);
        __sys_XinEntry_remove(selected_file);
        // while(1);
    }

    else if(strcmp(option, "rename"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 25;
        xprintf("New file name: ");
        xscanf("%s", new_name);
        __sys_XinEntry_move(selected_file, xin_get_current_path(new_name));

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
            status = __sys_xin_link_create(selected_file, xin_get_current_path(new_name));
        else
            status = __sys_xin_link_create(selected_file, new_name);
        
        free(new_name);
    }

    else if(strcmp(option, "copy"))
    {
        char* new_name = (char*)calloc(MAX_PATH);
        Screen.y = 23;

        xprintf("where do you want to create a copy entry: ");
        xscanf("%s", new_name);

        xprintf("selected file: %s\n", selected_file);
        xprintf("new file: %s\n", new_name[0] != '/' ? xin_get_current_path(new_name) : new_name);
        // msleep(3000);

        __sys_xin_copy(selected_file, new_name);
        
        free(new_name);
    }


    else if(strcmp(option, "edit"))
    {
        edit(selected_file);
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


}

void hfs(char* omg)
{

    
    if(strcmp(omg, "exit"))
    {
        exit_tui_test_app = true;
        return;
    }

    else if(strcmp(omg, ""))
    {
        table_t* fro = table_create(0, 15, 1, 20, black, white, 1);
        table_insert(fro, 0, "create", black, white, 0);
        table_add_handler(fro,handle_selected_option);
        table_row_select(fro);

        table_destroy(fro);
        // exit_tui_test_app = true;
        return;
    }

    else if(strcmp(omg, "..") && strcmp(app_current_folder, "/"))
        exit_tui_test_app = true;

    else if(strcmp(omg, ".."))
    {
        if(app_parent_folder_counter)
            app_parent_folder_counter--;
        strcpy(app_current_folder, app_parent_folder[app_parent_folder_counter]);
        free(app_parent_folder[app_parent_folder_counter]);
    }

    else
    {
        if(__sys_xin_folder_change(omg) != XANIN_ERROR)
        {
            app_parent_folder = (char**)realloc(app_parent_folder, sizeof(char*) * (app_parent_folder_counter + 1));
            app_parent_folder[app_parent_folder_counter] = (char*)calloc(80);
            strcpy(app_parent_folder[app_parent_folder_counter], app_current_folder);
            app_parent_folder_counter++;
            strcpy(app_current_folder, omg);
        }
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

        selected_file = omg;
        table_add_handler(fro,handle_selected_option);
        table_row_select(fro);

        table_destroy(fro);
    }

}

int explorer(char* parent_folder)
{
    app_current_folder = nullptr;
    app_parent_folder = nullptr;
    app_parent_folder_counter = 0;
    selected_file = nullptr;
    exit_tui_test_app = false;

    app_current_folder = (char*)calloc(80);

    if(!strlen(parent_folder))
        strcpy(app_current_folder, xin_get_current_directory()); 


    else if(parent_folder[strlen(parent_folder) - 1] != '/')
        parent_folder[strlen(parent_folder)] = '/';

    else if(xin_find_entry(parent_folder) != nullptr)
        strcpy(app_current_folder, xin_find_entry(parent_folder)->entry_path) ;

    else 
    { 
        xprintf("%zNo such directory: %s\n", stderr, parent_folder);
        strcpy(app_current_folder, "/");
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    app_parent_folder = (char**)calloc(sizeof(char*));

    while(KeyInfo.scan_code != F4_KEY) 
    {
        screen_clear();
        xprintf("CURRENT DIRECTORY: %s\n", app_current_folder);
        XinChildrenEntries* hoho = xin_get_children_entries(app_current_folder, false);
        table_t* fro = table_create(0,1, 10, 80, black, white, TUI_TEST_SITES);

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
                        table_insert(fro , j+1, "exit", black, red, i);
                        break;
                    }
                }
            }

            table_row_select(fro);
        }

        table_destroy(fro);
        free(hoho);
        free(fro);

        if(exit_tui_test_app)
            break;
    }

    for(int i = 0; i<app_parent_folder_counter; i++)
        free(app_parent_folder[i]);

    free(app_parent_folder);
    free(app_current_folder);

    return XANIN_OK;

}
