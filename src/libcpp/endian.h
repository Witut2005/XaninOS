
#include <stdint.h>

// uint16_t endian_switch(uint16_t nb);
// uint32_t endian_switch(uint32_t nb);


template <class T> 
T endian_switch(T bytes)
{
    T tmp = 0;

    for(int i = 0; i < sizeof(bytes); i++)
    {
        tmp = tmp << 8;
        tmp = tmp | (bytes & 0xFF);
        bytes = bytes >> 8;
    }

    return tmp;
    
}