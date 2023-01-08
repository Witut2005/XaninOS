
#include <tui/tui.h>
#include <libc/colors.h>

void hfs(char* omg)
{
    screen_clear();
    xprintf("selected option: %s\n", omg);
}

int my_tui_app(void)
{
    table_t* omg = table_create(5, 5, 5, 5, black, white);
    table_insert(omg, 0, "pi", black, white);

    table_t* fro = table_create(20, 5, 5, 15, black, white);
    table_insert(fro , 0, "ok", black, white);
    char* data = (char*)calloc(20);
    xprintf("data: %s", table_get_row_data(fro, 4, data));

    table_add_handler(fro, hfs);

    table_row_select(fro);

    while(KeyInfo.scan_code != ENTER);
    
}