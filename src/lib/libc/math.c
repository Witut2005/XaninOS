
#include <lib/libc/math.h>


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

    
float fpow(float base, float exponent)
{
    float result = 1.0;

    exponent = fabs(exponent);
    
    while (exponent != 0) 
    {
        result *= base;
        exponent--;
    }
    
    return result;
}

int abs(int num)
{
    if(num < 0)
        num = num * (-1);
    return num;
}

float fabs(float num)
{
    if(num < 0)
        num = num * (-1);
    return num;
}

