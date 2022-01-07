
#include <stdint.h>

void memcpy(uint8_t *dst, uint8_t* src, size_t size)
{
    for(int i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}


bool memcmp(uint8_t *dst, uint8_t* src, size_t size)
{
    for(int i = 0; i < size; i++)
        if(dst[i] != src[i])
            return false;

    return true;
    
}


void memset(uint8_t *dst, uint8_t value, size_t size)
{
    for(int i = 0; i < size; i++)
        dst[i] = value;
}
