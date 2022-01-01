

#ifndef IOSTD_H
#define IOSTD_H

#include "../headers/inttypes.h"
#include "../headers/colors.h"
#include "../headers/macros.h"
#include "../terminal/vty.h"
#include "../tetris/tetris.c"



#define VRAM 0xb8000


static char* keyString = "keyboard initalized succed :))\n";
static char HEX_LUT[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

static char* COMMAND;

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


//small print
void sprint(uint8_t backColor,uint8_t colors,char* str)
{

uint16_t* ptr;
ptr = (unsigned short*)(0xb8000) + ((80)*y);

for (int i = 0; str[i] != '\0'; i++)
    {
        ptr[i] = (uint16_t) str[i] + (((backColor << 4) | colors) << 8);
    }

   y++;

}


void putch(char c)
{
    uint16_t* ptr;
    ptr = (unsigned short*)(0xb8000) + (x*2) +((80)*y);
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


    incArr(arr,3,48);

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

void printHex(uint8_t x)
{
    char* mystr;
    putch(HEX_LUT[(x & 0xF0) >> 4]);
    putch(HEX_LUT[x & 0x0F]);

}
 
void printBCD(uint8_t x)
{

    uint16_t* ptr;

    ptr = (unsigned short*)(0xb8000) + x +  ((80)*y);
    *ptr = (uint16_t)( (((x & 0xf0) >> 4) + 48)  | (((red << 4) | white) << 8)); 
    x += 0x2;

    ptr = (unsigned short*)(0xb8000) + x +  ((80)*y);
    *ptr = (uint16_t)(((x & 0x0f) + 48)  | (((red << 4) | white) << 8)); 
    x += 0x2;

    y++;

    return;

}



//extended print :))
void xprint(char* str, uint8_t format)
{
    if(format == dec)
    {

    }

    else if(format == hex)
    {

    }

    else if(format == bin)
    {
        
    }

    else
    {

    }



}

#endif
