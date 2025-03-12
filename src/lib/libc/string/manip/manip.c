#include "../info/info.h"
#include "../local_macros.h"
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <stddef.h>
#include <stdint.h>

#define ASCII_CASE_OFFSET 32

char* toupper(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= ASCII_CASE_OFFSET;
        }
    }
    return str;
}

char* tolower(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] += ASCII_CASE_OFFSET;
        }
    }
    return str;
}

char* strcpy(char* dest, char const* src)
{
    LIBC_STR_EXIT_ON_EQUALS_ZERO(src, NULL);
    uint32_t length = strlen(src);
    memmove(dest, src, length);
    dest[length] = '\0';
    return dest;
}

// this strncpy implemtation put '\0' at the end
char* strncpy(char* dest, char const* src, size_t size)
{
    LIBC_STR_EXIT_ON_EQUALS_ZERO(size, NULL);

    uint32_t src_length = strlen(src);
    uint32_t length = src_length > size ? size : src_length;

    memcpy(dest, src, length);
    dest[length] = '\0';

    return dest;
}

char* strrev(char* str)
{
    int len = strlen(str);

    // for loop
    for (int i = 0, j = len - 1; i <= j; i++, j--)
    {
        // swapping characters
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    return str;
}

char* strdup(char const* str)
{
    char* ns = (char*)calloc(strlen(str) * sizeof(char));
    strcpy(ns, str);
    return ns;
}

char* strcat(bool dest_first, char* dest, char* src)
{
    // dest = dest + src
    if (dest_first)
    {
        memmove(&dest[strlen(dest)], src, strlen(src) + 1); // include '\0' too
    }

    // dest = src + dest
    else
    {
        char* ts = (char*)calloc(strlen(dest) + strlen(src));
        strcpy(ts, src);
        strcpy(&ts[strlen(ts)], dest);
        strcpy(dest, ts);
        free(ts);
    }
    return dest;
}

char* string_align_begin(char* const str, char filler, uint32_t count)
{

    uint32_t string_length = strlen(str);

    if (string_length >= count)
    {
        return str;
    }

    memmove(str + (count - string_length), str, string_length);

    int i;
    for (i = 0; i < count - string_length; i++)
        str[i] = filler;

    str[count] = '\0'; // put NULL terminator

    return str;
}

char* string_align_end(char* const str, char filler, uint32_t count)
{
    int i;
    for (i = strlen(str); i < count; i++)
    {
        str[i] = filler;
    }

    str[i] = '\0'; // put NULL terminator

    return str;
}
