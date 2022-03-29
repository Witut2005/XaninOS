#pragma once

#include <stdint.h>
#include <lib/string.h>



uint32_t pow(uint32_t a, uint32_t b)
{
    
    uint32_t operation_result = a;
    

    if(!b)
        return 1;
    
    else
        for(int i = 1; i < b; i++)
            operation_result = operation_result * a;

    return operation_result;

}

