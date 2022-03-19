#pragma once

#include <stdint.h>
#include <lib/string.h>



uint32_t pow(uint32_t a, uint32_t b)
{
    
    uint32_t result = a;
    

    if(!b)
        return 1;
    
    else
        for(int i = 0; i < b; i++)
            result = result * a;

    return result;

}

/*

uint32_t atoi(char* str) 
{
    uint32_t num = 0;

    for(int i = 0; str[i] != '\0'; i++)
        num = (num * 10) + str[i] - 48; 

    return num;

}

*/
