
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void memcpy(uint8_t* dest, uint8_t* src, size_t size)
{
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int32_t memcmp(const uint8_t* dest, uint8_t* src, size_t size)
{
    for (int i = 0; i < size; i++) {
        if (dest[i] != src[i]) {
            return dest[i] - src[i];
        }
    }
    return 0;
}

bool bmemcmp(const uint8_t* dest, uint8_t* src, size_t size)
{
    return memcmp(dest, src, size) == 0;
}

void memset(uint8_t* dest, uint8_t value, size_t size)
{
    for (int i = 0; i < size; i++) {
        dest[i] = value;
    }
}

void memmove(uint8_t* dest, uint8_t* src, size_t size)
{

    if (dest == src) {
        return;
    }

    //src > dest
    else if (dest < src) {
        for (int i = 0; i < size; i++) {
            dest[i] = src[i];
        }
    }

    // dst > src
    else
    {
        for (int i = size - 1; i >= 0; i--) {
            dest[i] = src[i];
        }
    }

}

void memcpy_with_skip(uint8_t* dest, uint8_t* src, size_t size, uint32_t skip)
{
    if (skip == 0) {
        return memcpy(dest, src, size);
    }

    else {
        uint32_t counter = skip;
        for (int si = 0, di = 0; si < size; si++) {
            if (counter == 0) {
                counter = skip;
                continue;
            }
            else {
                dest[di++] = src[si];
                counter--;
            }
        }
    }
}