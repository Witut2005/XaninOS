#pragma once

#include <stdint.h>
#include "../lib/string.h"



uint8_t pow(volatile uint8_t a, volatile uint8_t b)
{
    
    static uint8_t result;
    result = a; //stack doesnt work :flushed:

    if(!b)
        return 1;
    
    else
        for(int i = 1; i < b; i++)
            result = result * a;

    return result;

}


uint32_t atoi(char* str) 
{


    reverseStr(str);

    uint32_t num = 0;
    uint8_t buf;
    uint8_t ctr = 0;


    for(int i = strlen(str)-1; i >= 0; i--)
        str[i] = str[i] - 48;
    


    for(int i = 0; strlen(str)-1; i++)
        num += str[i] * pow(str[i],i);
    

}