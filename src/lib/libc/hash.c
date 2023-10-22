
#include "./hash.h"
#include "./string.h"

uint32_t jhash_with_length(const char* key, uint32_t length)
{
    uint32_t hash = 0;

    for(uint32_t i = 0; i < length; i++)
    {
        hash += key[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

uint32_t jhash(const char* key)
{
    return jhash_with_length(key, strlen(key));
}
