
#include <programs/hexeditor/hexeditor.h>
#include <sys/input/input.h>

static char* data_pointer;
static uint32_t data_pointer_position;
static uint8_t tmp; 

#define HEXEDITOR_BYTES_IN_ROW 16
#define HEXEDITOR_SCREEN_ROW_WIDTH 46

void hexeditor_letters_refresh(void)
{
    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        Screen.x = 51;
        Screen.y = i;

        for(int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            putchar(data_pointer[ (HEXEDITOR_BYTES_IN_ROW * i) + j]);

        xprintf("\n");
    }
}

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

        data_pointer_position -= HEXEDITOR_BYTES_IN_ROW;

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

        data_pointer_position += HEXEDITOR_BYTES_IN_ROW;

    }

    else if(x.scan_code == ARROW_RIGHT)
    {
        
        if((uint32_t)&Screen.cursor[Screen.y][Screen.x + 1] > VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION - 8)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        Screen.x++;

        if(Screen.x >= HEXEDITOR_SCREEN_ROW_WIDTH)
        {
            Screen.x = 0;
            Screen.y++;
            data_pointer_position++;
        }

        else if(Screen.cursor[Screen.y][Screen.x] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
        {
            Screen.x++;
            data_pointer_position++;
        }

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

    }

    else if(x.scan_code == ARROW_LEFT)
    {

        if((uint32_t)&Screen.cursor[Screen.y][Screen.x - 1] < VGA_TEXT_MEMORY)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
    
        if(Screen.x == 0)
        {
            Screen.x = HEXEDITOR_SCREEN_ROW_WIDTH;
            Screen.y--;
            data_pointer_position--;
        }

        else if(Screen.cursor[Screen.y][Screen.x - 1] == (uint16_t)( ' '| (((black << 4) | white) << 8)))
        {
            Screen.x = Screen.x - 2;
            data_pointer_position--;
        }

        else 
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
        

        if(Screen.x >= HEXEDITOR_SCREEN_ROW_WIDTH)
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

        uint8_t x_tmp = Screen.x;
        uint8_t y_tmp = Screen.y;

        hexeditor_letters_refresh();

        Screen.x = x_tmp;
        Screen.y = y_tmp;

    }  
}


int hexeditor(char* file_name, char* options)
{

    data_pointer = (char*)NULL;
    data_pointer_position = 0;
    tmp = 0; 

    uint32_t hexeditor_offset = 0;

    if(bstrcmp(options, "-offset"))
        hexeditor_offset = strtoi(argv[3], HEXADECIMAL);

    XinEntry* file = fopen(file_name, "rw");

    if(file == NULL)
    {
        xprintf("Can't open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    data_pointer = (char*)calloc(VGA_SCREEN_RESOLUTION);

    screen_clear();

    fseek(file, hexeditor_offset);
    fread(file, data_pointer, VGA_SCREEN_RESOLUTION);

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        for(int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            xprintf("%mX ", data_pointer[ (HEXEDITOR_BYTES_IN_ROW * i) + j]);

        xprintf("%z | ", OUTPUT_COLOR_SET(black, green));

        for(int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            putchar(data_pointer[ (HEXEDITOR_BYTES_IN_ROW * i) + j]);

        xprintf("\n");
    }
    
    Screen.x = 0;
    Screen.y = 0;

    while(!app_exited)hexeditor_input(inputg());

    fseek(file, hexeditor_offset);
    fwrite(file, data_pointer, 512);
    fclose(&file);
    free(data_pointer);
    return XANIN_OK;


}