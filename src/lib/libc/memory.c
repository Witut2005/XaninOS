
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

void memmove(uint8_t *dst, uint8_t* src, size_t size)
{
    
    if(dst == src)
        return;

    else if(dst < src)
    {
        for(int i = 0; i < size; i++)
            dst[i] = src[i];
    }

    else  // dst > src
    {
        for(int i = size - 1; i >= 0; i--)
            dst[i] = src[i];
    }

}