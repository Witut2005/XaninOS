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

