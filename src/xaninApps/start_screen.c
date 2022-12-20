
uint16_t string_x = 5;
uint16_t string_y = 0;
bool if_down = true;
bool if_right = true;

static inline void align_xanin_os_string(void)
{
    for(int i = 0; i < string_x; i++)
        xprintf(" ");
}

static inline uint8_t get_new_front_color(void)
{
    uint8_t tmp = (rand() % 15) + 1;
    while(tmp == logo_front_color)
    {
        tmp = (rand() % 15) + 1;
    }
    return tmp;
}

static inline void print_xanin_os_string(void)
{
    for(int i = 0; i < string_y; i++)
        xprintf("\n");

    char* first_row = " __  __          _      ___  ___\n";
    char* second_row = " \\ \\/ /__ _ _ _ (_)_ _ / _ \\/ __|\n";
    char* third_row = "  >  </ _` | \' \\| | \' \\ (_) \\__ \\\n";
    char* fourth_row= " /_/\\_\\__,_|_||_|_|_||_\\___/|___/\n";

    align_xanin_os_string();
    xprintf("%z%s", set_output_color(logo_back_color, logo_front_color), first_row);
    align_xanin_os_string();
    xprintf("%z%s", set_output_color(logo_back_color, logo_front_color), second_row);
    align_xanin_os_string();
    xprintf("%z%s", set_output_color(logo_back_color, logo_front_color), third_row);
    align_xanin_os_string();
    xprintf("%z%s", set_output_color(logo_back_color, logo_front_color), fourth_row);

}

int start_screen(void)
{

screen_clear();

logo_front_color = white;

print_xanin_os_string();

    while(KeyInfo.scan_code != ENTER)
    {

        if(!if_down)
        {
            if(string_y) 
            {
                screen_clear();
                string_y--;
                print_xanin_os_string();
            }

            else
            {
                logo_front_color = get_new_front_color();
                if_down = true;
            }

        }

        if(if_down)
        {
            if(string_y < VGA_HEIGHT - 4) 
            {
                screen_clear();
                string_y++;
                print_xanin_os_string();
            }

            else
            {
                logo_front_color = get_new_front_color();
                if_down = false;
            }

        }


        if(if_right)
        {
            if(string_x < 47) 
            {
                screen_clear();
                string_x++;
                print_xanin_os_string();
            }

            else
            {
                logo_front_color = get_new_front_color();
                if_right = false;
            }

        }

        if(!if_right)
        {
            if(string_x) 
            {
                screen_clear();
                string_x--;
                print_xanin_os_string();
            }

            else
            {
                logo_front_color = get_new_front_color();
                if_right = true;
            }

        }


        msleep(300);
    }

}