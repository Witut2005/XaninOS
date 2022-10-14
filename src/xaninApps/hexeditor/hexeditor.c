
#include <xaninApps/hexeditor/hexeditor.h>


char* data_pointer;
uint32_t data_pointer_position = 0;

uint8_t tmp; 

void hexeditor_input(xchar x)
{

    if(x.scan_code == F4_KEY || x.scan_code == ESC)
        app_exited = true;

    else if(x.scan_code == ARROW_UP)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        
        if((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] < VGA_TEXT_MEMORY)
        {

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
            return;
        }

        Screen.y--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

        //data_pointer_position -= 26;

        data_pointer_position -= 26;

    }

    else if(x.scan_code == ARROW_DOWN)
    {
        

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        
        if((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] >= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION)
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
            return;
        }

        Screen.y++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

        data_pointer_position += 26;

    }

    else if(x.scan_code == ARROW_RIGHT)
    {
        
        if((uint32_t)&Screen.cursor[Screen.y][Screen.x + 1] > VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION - 8)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if(Screen.x == 76)
        {
            Screen.x = 0x0;
            Screen.y++;
            data_pointer_position++;
        }

        if(Screen.cursor[Screen.y][Screen.x + 1] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
        {
            Screen.x++;
            data_pointer_position++;
        }

        Screen.x++;
        

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

    }

    else if(x.scan_code == ARROW_LEFT)
    {

        if((uint32_t)&Screen.cursor[Screen.y][Screen.x - 1] < VGA_TEXT_MEMORY)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
    
        if(Screen.x == 0)
        {
            Screen.x = 78;
            Screen.y--;
            data_pointer_position--;
        }

        if(Screen.cursor[Screen.y][Screen.x - 1] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
        {
            Screen.x--;
            data_pointer_position--;
        }

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));


    }

    else if(x.scan_code == F4_KEY)
    {
        app_exited = true;
        return;
    }
    
    else if(x.character != '\0')
    {
        if((uint32_t)&Screen.cursor[Screen.y][Screen.x] >= VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION - 7)
        {
            Screen.x--;
            return;
        }
        

        if(Screen.x >= 77)
        {
            Screen.x = 0;
            Screen.y++;
            data_pointer_position++;
        }

        if(x.character >= 'a' && x.character <= 'f')
        {
            if(Screen.cursor[Screen.y][Screen.x + 1] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
            {
                data_pointer[data_pointer_position] &= 0xF0;
                tmp = KeyInfo.character - 'a' + 0xa;
                data_pointer[data_pointer_position] += tmp;
            }
            
            else
            {
                data_pointer[data_pointer_position] &= 0x0F;
                tmp = ((KeyInfo.character - 'a' + 0xa) << 4);
                data_pointer[data_pointer_position] += tmp;
            }
         
            xprintf("%c", x.character);
        }

        else if(x.character >= '0' && x.character <= '9')
        {
            data_pointer[data_pointer_position] = 0x33;
            if(Screen.cursor[Screen.y][Screen.x + 1] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
            {

                data_pointer[data_pointer_position] &= 0xF0;
                tmp = KeyInfo.character - '0';
                data_pointer[data_pointer_position] = data_pointer[data_pointer_position] + tmp;
            }
            
            else
            {
                data_pointer[data_pointer_position] &= 0x0F;
                tmp = ((KeyInfo.character - '0') << 4);
                data_pointer[data_pointer_position] = data_pointer[data_pointer_position] + tmp;
            }
            
            xprintf("%c", x.character);
        
        }
            
        if(Screen.cursor[Screen.y][Screen.x] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
        {
            Screen.x++;
            data_pointer_position++;
        }

        x.character = 0;

    }  
}


void hexeditor(char* file_name)
{



    xin_entry* file = fopen(file_name, "rw");

    if(file == nullptr)
    {
        xprintf("Can't open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return;
    }

    data_pointer = (char*)calloc(VGA_SCREEN_RESOLUTION);


    screen_clear();

    read(file, data_pointer, VGA_SCREEN_RESOLUTION);

    for(int i = 0; i < 28; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            xprintf("%mX ", data_pointer[ (26 * i) + j]);
        }
        xprintf("\n");
    }

    fseek(file, 0);
    Screen.x = 0;
    Screen.y = 0;

    while(!app_exited)hexeditor_input(inputg());

    screen_clear();

    fseek(file, 0);
    write(file, data_pointer, 512);
    fclose(&file);
    free(data_pointer);


}