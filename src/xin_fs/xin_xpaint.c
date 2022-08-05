

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <libc/string.h>

#define VGA_SCREEN_RESOLUTION 4480


static uint8_t current_color;
static uint16_t selected_cell;


void xpaint_input_color(xchar x)
{

    if(KeyInfo.is_alt)
    {

        Screen.cursor[Screen.y][Screen.x] = Screen.cursor[Screen.y][Screen.x] & 0xFF00;

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


        if(x.scan_code == ARROW_UP)
        {

            if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] >= VGA_TEXT_MEMORY)
                Screen.y--;
        }

        else if(x.scan_code == ARROW_DOWN)
        {
            if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] <= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
                Screen.y++;
        }

        else if(x.scan_code == ARROW_RIGHT)
        {
            Screen.x++; 

            if(Screen.x == 80)
            {
                Screen.x = 0x0;
                Screen.y++;
            }
        
        }

        else if(x.scan_code == ARROW_LEFT)
        {
            if(Screen.x == 0)
            {
                Screen.x = 80;
                Screen.y--;
            }

            Screen.x--;

        }
        // xprintf("morb");
        
        return;
    
    }


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
        app_exited = true;

    else if(x.scan_code == ARROW_UP)
    {
        if((uint8_t)selected_cell == '_')
            selected_cell = (selected_cell & 0xFF00) | ' ';

        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] >= VGA_TEXT_MEMORY)
            Screen.y--;
   

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);        
    }

    else if(x.scan_code == ARROW_DOWN)
    {
        if((uint8_t)selected_cell == '_')
            selected_cell = (selected_cell & 0xFF00) | ' ';

        Screen.cursor[Screen.y][Screen.x] = selected_cell;
        
        if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] <= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
            Screen.y++;

        selected_cell = Screen.cursor[Screen.y][Screen.x];
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)('_' | ((black << 4) | white) << 8);   
    }

    else if(x.scan_code == ARROW_RIGHT)
    {
        if((uint8_t)selected_cell == '_')
            selected_cell = (selected_cell & 0xFF00) | ' ';

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
        if((uint8_t)selected_cell == '_')
            selected_cell = (selected_cell & 0xFF00) | ' ';

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

        if(!KeyInfo.is_ctrl)
        {

            uint8_t current_letter = (uint8_t)selected_cell;

            if(current_letter == '_')
                current_letter = ' ';

            switch(x.scan_code)
            {
                case _0_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((black << 4) | black) << 8)); current_color    = black; break;
                case _1_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((blue << 4) | black) << 8)); current_color     = blue; break;
                case _2_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((green << 4) | black) << 8)); current_color    = green;break;
                case _3_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((cyan << 4) | black) << 8)); current_color     = cyan; break;
                case _4_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((red << 4) | black) << 8)); current_color      = red; break;
                case _5_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((magenta << 4) | black) << 8)); current_color  = magenta; break;
                case _6_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((brown << 4) | black) << 8)); current_color    = brown; break;
                case _7_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lgray << 4) | black) << 8)); current_color    = lgray; break;
                case _8_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((dgray << 4) | black) << 8)); current_color    = dgray; break;
                case _9_KEY: Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lblue << 4) | black) << 8)); current_color    = lblue; break;
                case A_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lgreen << 4) | black) << 8)); current_color   = lgreen; break;
                case B_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lcyan << 4) | black) << 8)); current_color    = lcyan; break;
                case C_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lred << 4) | black) << 8)); current_color     = lred; break;
                case D_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((lmagenta << 4) | black) << 8)); current_color = lmagenta; break;
                case E_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((yellow << 4) | black) << 8)); current_color   = yellow; break;
                case F_KEY : Screen.cursor[Screen.y][Screen.x] = (uint16_t) (current_letter | (((white << 4) | black) << 8)); current_color    = white; break;
            }

            selected_cell = Screen.cursor[Screen.y][Screen.x];
        
        }

        else
        {
            xprintf("%c", KeyInfo.character);
        }

    }

    KeyInfo.scan_code = 0x0;

}


void xin_xpaint(char* file_name)
{


    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return;
    }

    else
    {
    
        uint16_t* data_pointer = (uint16_t*)(xin_file->starting_sector * SECTOR_SIZE);
        screen_clear();

        uint16_t* screen_cell = (uint16_t*)VGA_TEXT_MEMORY;

        for(int i = 0; i < VGA_SCREEN_RESOLUTION / 2; i++)
            screen_cell[i] = (uint16_t) (data_pointer[i]);

        Screen.x = 0x0;
        Screen.y = 0x0;

        selected_cell = Screen.cursor[Screen.y][Screen.x];

        while(!app_exited)
        {
            xpaint_input_color(inputg());
        }   

        uint32_t file_data_counter = 0x1;

        uint8_t* screen_ptr = (uint8_t*)VGA_TEXT_MEMORY;

        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++, screen_ptr += 2)
            write(xin_file, screen_ptr + 1, 1);
        
        xin_file->entry_size = file_data_counter;
        

    }

    keyboard_handle = nullptr;
    return;

}
