

#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <headers/colors.h>
#include <headers/macros.h>
#include <terminal/vty.h>
#include <lib/stdlibx.h>



#define set_output_color(x,y) (x << 4 | y)
#define cursor_set_position(x, y) (x << 8 | y)

#define VRAM VGA_TEXT_MEMORY

#define VGA_SCREEN_RESOLUTION 4480

static char* keyString = "keyboard initalized succed :))\n";
static char HEX_LUT[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

bool use_backspace = false;

void screen_clear(void)
{
    uint16_t* ptrTmp = (uint16_t*)VGA_TEXT_MEMORY;
    for(int i = 0; i < (80 * 28); i++)
    {
        *ptrTmp = '\0';
        ptrTmp++;
    }

    x = 0;
    y = 0;
    Screen.x = 0;
    Screen.y = 0;
}


/* small print */
void sprint(uint8_t backColor,uint8_t colors,char* str)
{

cursor = (unsigned short*)(VGA_TEXT_MEMORY) + ((80)*y);

    for (int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == '\n')
        {
            y++;
            cursor = (unsigned short*)(VGA_TEXT_MEMORY) + ((80)*y) - 1;
            continue;
        }

        cursor[i] = (uint16_t) (str[i] + (((backColor << 4) | colors) << 8));
    }

    y++;

}

uint8_t x_putch = 0x0;

/* put character */
void putch(char c)
{

    

    uint16_t* ptr;
    
    ptr = (unsigned short*)(VGA_TEXT_MEMORY + x_putch);
    *ptr = (uint16_t) c + (((black << 4) | white) << 8);

    x_putch+=2;


}



void print_hex_number(uint8_t x)
{
    char* mystr;
    putch(HEX_LUT[(x & 0xF0) >> 4]);
    putch(HEX_LUT[x & 0x0F]);

}

void print_bcd_number(uint8_t x)
{

    uint16_t* ptr;

    ptr = (unsigned short*)(VGA_TEXT_MEMORY) + x +  ((80)*y);
    *ptr = (uint16_t)( (((x & 0xf0) >> 4) + 48)  | (((red << 4) | white) << 8));
    x += 0x2;

    ptr = (unsigned short*)(VGA_TEXT_MEMORY) + x +  ((80)*y);
    *ptr = (uint16_t)(((x & 0x0f) + 48)  | (((red << 4) | white) << 8));
    x += 0x2;

    y++;

    return;

}

char putchar(char character)
{

    if(character < 0x20)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (0x0 + (((black << 4) | white) << 8));
    
        Screen.x++;

        if(x == 80)
        {    
            Screen.x = 0x0;
            Screen.y++;
        }
        return false;
    }

    Screen.cursor[Screen.y][Screen.x] = (unsigned short*)(VGA_TEXT_MEMORY) + x + ((80)*y);
    Screen.cursor[Screen.y][Screen.x] = (uint16_t) (character + (((black << 4) | white) << 8));
        
    Screen.x++;

    if(x == 80)
    {    
        Screen.x = 0x0;
        Screen.y++;
    }
           

    return character;

}



void xprintf(char* str, ... )
{

    char tmp[128];

    for(int i = 0; i < sizeof(tmp); i++)
        tmp[i] = '\0';

    char* tmpPtr = tmp;
    uint32_t number;

    char* stringPtr;

    va_list args;
    va_start(args,str);



    uint32_t strCounter = 0;

    uint8_t backgroundColor = black;
    uint8_t fontColor = white;


    while(str[strCounter])
    {

        for(int i = 0; i < sizeof(tmp); i++)
            tmp[i] = '\0';

        if(str[strCounter] == '%')
        {

            strCounter++;
            switch(str[strCounter])
            {
                case 'd':
                {
                    number = va_arg(args,int);

                    int_to_str(number,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {                        
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    break;
                }

                case 'b':
                {
                    number = va_arg(args,int);

                    tmpPtr = bin_to_str(number,tmp);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    { 
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    break;
                }

                    

                case 's':
                {

                    stringPtr = va_arg(args,char*);


                    for(int i = 0; stringPtr[i] != '\0'; i++)
                    {
                        if(stringPtr[i] == '\n')
                        {            
                            Screen.x = 0x0;
                            Screen.y++;
                            continue;
                        }

                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (stringPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    

                    }

                    break;

                }

                case 'c':
                {

                    char character;
                    character = (char)va_arg(args,int);
                    
                    if(character == '\n')
                    {            
                        Screen.x = 0x0;
                        Screen.y++;
                        continue;
                    }

                    if(character == '\0')
                    {
                        return;
                    }

                    Screen.cursor[Screen.y][Screen.x] = (uint16_t) (character + (((backgroundColor << 4) | fontColor) << 8));

                    if(Screen.x == 80)
                    {
                        Screen.y++;
                        Screen.x = 0x0;
                    }

                    Screen.x++;

                    break;
                }

                case 'z':
                {
                    fontColor = (uint8_t)va_arg(args,int);
                    backgroundColor = (fontColor & 0xf0) >> 4;
                    fontColor = fontColor & 0x0f;
                    break;
                }

                case 'x':
                {
                    uint32_t number_hex = va_arg(args,uint32_t);
                    int_to_hex_str(number_hex,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    
                    break;

                }
                
                case 'X':
                {
                    number = va_arg(args,int);
                    int_to_hex_str(number,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }

                    toupper(tmpPtr);
                    
                    break;

                }
                
                case 'o':
                {
                    number = va_arg(args,int);
                    int_to_oct_str(number,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {
                        Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        if(Screen.x == 80)
                        {
                            Screen.y++;
                            Screen.x = 0x0;
                        }
                        Screen.x++;
                    }
                    break;
                }

                case 'h':
                {
                    number = (uint16_t)va_arg(args,uint32_t);
                    Screen.x = (number >> 8) & 0xFF;
                    Screen.y = number & 0xFF;
                    break;
                }

                case 'm':
                {

                    strCounter++;
                    switch(str[strCounter])
                    {
                        case 'x':
                        {

                            uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                            xint_to_hex_str(number_hex,tmpPtr, sizeof(uint8_t));

                            for(int i = 0; tmpPtr[i] != '\0'; i++)
                            {
                                Screen.cursor[Screen.y][Screen.x] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                                
                                if(Screen.x == 80)
                                {
                                    Screen.y++;
                                    Screen.x = 0x0;
                                }
                                
                                Screen.x++;
                            }

                    
                            break;                            

                        }
                    
                    }


                    break;
                }


            }

            strCounter++;
        }

        else if(str[strCounter] == '\n')
        {            
            Screen.x = 0x0;
            Screen.y++;
            strCounter++;
        }

        else if(str[strCounter] == '\r')
        {
            x = 0;
            cursor = (uint16_t*)(VGA_TEXT_MEMORY) + ((80)*y);
            strCounter++;
        }

        else if(str[strCounter] == '\t')
        {
            Screen.x += 3;
            strCounter++;
        }

        else if(str[strCounter] == '\\')
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)('\\' + (((backgroundColor << 4) | fontColor) << 8));
            strCounter++;
        }

        else
        {
            //cursor[bufCounter] = (uint16_t) (str[strCounter] + (((backgroundColor << 4) | fontColor) << 8));

            //x++;

            Screen.cursor[Screen.y][Screen.x] = (uint16_t) (str[strCounter] + (((backgroundColor << 4) | fontColor) << 8));
            Screen.x++;
            strCounter++;
        }



    }

    va_end(args);

}



void xscanf(char* str, ... )
{
    no_enter = true;
    keyboard_scan_code = 0x0;

    uint32_t str_counter = 0x0;
    uint32_t counter = 0x0;

    char* string_pointer;

    va_list args;
    va_start(args, str);

    char buffer[50];

    for(int i = 0; i < 50;i++)
        command_buffer[i] = '\0';

    for(int i = 0; i < 50;i++)
        buffer[i] = '\0';

    index = 0x0;

    use_backspace = true;

    start:

    while(1)
    {

        if(KeyInfo.is_bspc)
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

            if((char)Screen.cursor[Screen.y][Screen.x - 1] == character_blocked && character_blocked != '\0')
            {
                goto start;
            }



            if(!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
                return;
            }

            Screen.x--;



            if(index)
                index--;

            command_buffer[index] = '\0';
            Screen.cursor[Screen.y][Screen.x] = '\0';

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | white) << 8));


            msleep(10);
            KeyInfo.is_bspc = false;
            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if(KeyInfo.scan_code == ENTER)
        {
            while(str[str_counter])
            {
                if(str[str_counter] == '%')
                {
                    str_counter++;
                    switch(str[str_counter])
                    {
                        case 's':
                        {

                            string_pointer = va_arg(args, char*);

                            for(int i = 0; string_pointer[i] != '\0'; i++)
                                string_pointer[i] = '\0';

                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                                
                                
                            for(int i = 0; buffer[i] != '\0' && buffer[i] != ' '; i++)
                            {
                                if((buffer[i] > 127) || (buffer[i] < 0x20))
                                {
                                    string_pointer[i] = '\0';
                                    goto end;
                                }

                                string_pointer[i] = buffer[i];
                            }

                            end:
                            
                            for(int i = 0x0; i < 50;i++)
                                buffer[i] = '\0';

                            counter++;
                            break;
                        }


                        case 'd':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 10);                       

                            break;
                        }

                        case 'x':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 16);                       

                            break;
                        }
                    

                        case 'b':
                        {

                            uint32_t* number = va_arg(args, uint32_t*);
                            
                            for(int i = 0x0; keyboard_command[counter] != '\0' && keyboard_command[counter] != ' '; i++)
                            {
                                buffer[i] = keyboard_command[counter];
                                counter++;
                            }
                            
                            *number = strtoi(buffer, 2);                       

                            break;
                        }


                    }

                str_counter++;

                }

                else
                {
                    str_counter++;
                }

            }

        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';


        for(int i = 0x0; i < 50;i++)
            buffer[i] = 0x0;

        KeyInfo.scan_code = 0x0;

        use_backspace = false;

        return;

        }

        else if(KeyInfo.character)
        {
            char tmp = getchar();
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));
            char character_saved = (char)(Screen.cursor[Screen.y][Screen.x]);
            
            //Screen.cursor[Screen.y][Screen.x + 1] = (uint16_t)(' ' + (((black << 4) | white) << 8));



            xprintf("%c", tmp);

            letters_refresh_add(&Screen.cursor[Screen.y][Screen.x], character_saved);
            keyboard_refresh_add(index, character_saved);


            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | white) << 8));
            keyboard_command[index] = tmp;
            index++;

        }    
    }

    use_backspace = false;


}
