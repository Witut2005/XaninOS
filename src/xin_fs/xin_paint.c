

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <libc/string.h>

#define VGA_SCREEN_RESOLUTION 4480

static uint8_t current_color;
static uint16_t selected_cell;

void paint_input(xchar x)
{


    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] < VGA_TEXT_MEMORY)
    {
        Screen.x = 0x0;
        Screen.y = 0x0;
        selected_cell = '\0';
    }

    if((uint32_t)&Screen.cursor[Screen.y][Screen.x] >= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
    {
        Screen.x = 79;
        Screen.y = 27;
        selected_cell = '\0';
    }

    if(x.scan_code == F4_KEY)
    {
        app_exited = true;
    }

    else if(x.scan_code == ARROW_UP)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] >= VGA_TEXT_MEMORY)
            Screen.y--;
   

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);        
    }

    else if(x.scan_code == ARROW_DOWN)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] <= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
            Screen.y++;

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);   
    }

    else if(x.scan_code == ARROW_RIGHT)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;


        Screen.x++; 

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
         
        if(Screen.x == 80)
        {
            Screen.x = 0x0;
            Screen.y++;
        }

        return;
    
    }

    else if(x.scan_code == ARROW_LEFT)
    {
        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if(Screen.x == 0)
        {
            Screen.x = 80;
            Screen.y--;
        }

        
        Screen.x--;

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    else if(x.scan_code == F3_KEY)
    {
        selected_cell = 0x0;
        screen_clear();
    }

    else
    {
        switch(x.scan_code)
        {
            case _0_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((black << 4) | black) << 8)); current_color    = black; break;
            case _1_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((blue << 4) | black) << 8)); current_color     = blue; break;
            case _2_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((green << 4) | black) << 8)); current_color    = green;break;
            case _3_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((cyan << 4) | black) << 8)); current_color     = cyan; break;
            case _4_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((red << 4) | black) << 8)); current_color      = red; break;
            case _5_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((magenta << 4) | black) << 8)); current_color  = magenta; break;
            case _6_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((brown << 4) | black) << 8)); current_color    = brown; break;
            case _7_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lgray << 4) | black) << 8)); current_color    = lgray; break;
            case _8_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((dgray << 4) | black) << 8)); current_color    = dgray; break;
            case _9_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lblue << 4) | black) << 8)); current_color    = lblue; break;
            case A_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lgreen << 4) | black) << 8)); current_color   = lgreen; break;
            case B_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lcyan << 4) | black) << 8)); current_color    = lcyan; break;
            case C_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lred << 4) | black) << 8)); current_color     = lred; break;
            case D_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((lmagenta << 4) | black) << 8)); current_color = lmagenta; break;
            case E_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((yellow << 4) | black) << 8)); current_color   = yellow; break;
            case F_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x20 + (((white << 4) | black) << 8)); current_color    = white; break;
        }

        if(KeyInfo.character)
            selected_cell = (uint16_t) (' ' + (((current_color << 4) | black) << 8));
    }

    KeyInfo.scan_code = 0x0;

}


int xin_paint(char* file_name)
{
    screen_clear();


    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else
    {
    
        char* data_pointer = (char*)calloc(VGA_SCREEN_RESOLUTION);//xin_file->starting_sector * SECTOR_SIZE;
        char* data_pointer_save = data_pointer;
        read(xin_file, data_pointer, VGA_SCREEN_RESOLUTION);
        
        uint16_t* bruh_moment = VGA_TEXT_MEMORY;


        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
            bruh_moment[i] = (uint16_t)(' ' | ((data_pointer[i]) << 8));

        /*
        for(int i = 0; i < (VGA_SCREEN_RESOLUTION / 2); i++)
            xprintf("%z ", set_output_color(data_pointer[i + 1], white));
        */
        
        Screen.x = 0x0;
        Screen.y = 0x0;

        selected_cell = Screen.cursor[Screen.y][Screen.x];

        while(!app_exited)
            paint_input(inputg());

        uint32_t file_data_counter = 0x1;

        data_pointer = data_pointer_save;

        
        uint8_t* screen_ptr = (uint8_t*)VGA_TEXT_MEMORY;

        fseek(xin_file, 0);
        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++, screen_ptr += 2)
        {
            write(xin_file, screen_ptr + 1, 1);
        }
        
        xin_file->entry_size = file_data_counter;
        

    }

    keyboard_handle = nullptr;
    fclose(&xin_file);
    return XANIN_OK;
    // 11:48:24

}
