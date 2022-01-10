

#ifndef IOSTD_H
#define IOSTD_H

#include <stdarg.h>
#include <stdint.h>
#include <headers/colors.h>
#include <headers/macros.h>
#include <terminal/vty.h>
#include <tetris/tetris.c>



#define set_output_color(x,y) (x << 4 | y)
#define VRAM VGA_TEXT_MEMORY


static char* keyString = "keyboard initalized succed :))\n";
static char HEX_LUT[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

char comBuf[30];
char* COMMAND;



static char znakk;

void clearScr(void)
{
    char* ptrTmp = (char*)VRAM;
    for(int i = 0; i < SCREEN_RESOLUTION*2; i++)
    {
        *ptrTmp = 0x0;
        ptrTmp++;
    }
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


/* put character */
void putch(char c)
{
    uint16_t* ptr;
    ptr = (unsigned short*)(VGA_TEXT_MEMORY) + (x*2) +((80)*y);
    *ptr = (uint16_t)c | (((black << 4) | white) << 8); 
    
    x++;

    return;
}


void printNum(uint8_t x)
{
    uint8_t arr[4];

    arr[3] = '\0';

    arr[2] = x % 10;
    x = x / 10;
    
    arr[1] = x % 10;
    x = x / 10;
    
    arr[0] = x % 10;


    increment_array(arr,3,48);

    bool notZero;

    uint16_t* ptr;
    

    for(uint8_t i = 0; i < 3; i++)
    {
        char charakter = arr[i];
        notZero = true;
        putch(charakter);
    }
    
    y++;
    x = 0;

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




void xprintf(char* str, ... )
{

    char tmp[20];
    char* tmpPtr = tmp;
    uint32_t number;
    
    char* stringPtr;

    va_list args;
    va_start(args,str);

    

    uint32_t strCounter = 0;
    uint32_t bufCounter = 0;

    uint8_t backgroundColor = black;
    uint8_t fontColor = white;

    cursor = (unsigned short*)(VGA_TEXT_MEMORY) + x +  ((80)*y);

    while(str[strCounter])
    {
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
                        cursor[bufCounter] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        bufCounter++;
                        x++;
                    }

                    break;
                }

                case 's':
                {    
                
                    stringPtr = va_arg(args,char*);
                    
                    for(int i = 0; stringPtr[i] != '\0'; i++)
                    {
                        cursor[bufCounter] = (uint16_t) (stringPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        bufCounter++;
                        x++;
                    }

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
                    number = va_arg(args,int);
                    int_to_hex_str(number,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {
                        cursor[bufCounter] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        bufCounter++;
                        x++;
                    }

                }

                case 'o':
                {
                    number = va_arg(args,int);
                    int_to_oct_str(number,tmpPtr);

                    for(int i = 0; tmpPtr[i] != '\0'; i++)
                    {
                        cursor[bufCounter] = (uint16_t) (tmpPtr[i] + (((backgroundColor << 4) | fontColor) << 8));
                        bufCounter++;
                        x++;
                    }
                }
        

                
            }

            strCounter++;
        }

        else if(str[strCounter] == '\n')
        {
            y++;
            x = 0;
            cursor = (unsigned short*)(VGA_TEXT_MEMORY) + ((80)*y);
            strCounter++;   
        }

        else if(str[strCounter] == '\r')
        {
            x = 0;
            cursor = (unsigned short*)(VGA_TEXT_MEMORY) + ((80)*y);
            strCounter++;
        }

        else if(str[strCounter] == '\t')
        {
            x += 3;
            cursor += 3;
            strCounter++;
        }

        else 
        {
            cursor[bufCounter] = (uint16_t) (str[strCounter] + (((backgroundColor << 4) | fontColor) << 8));
            strCounter++;   
            bufCounter++;
            x++;
        }

       

    }
    
    va_end(args);

}


#endif
