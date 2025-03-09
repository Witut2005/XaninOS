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
    EXIT_ON_EQUALS_ZERO(src, NULL);
    uint32_t length = strlen(src);
    memmove(dest, src, length);
    dest[length] = '\0';
    return dest;
}

// this strncpy implemtation put '\0' at the end
char* strncpy(char* dest, char const* src, size_t size)
{
    EXIT_ON_EQUALS_ZERO(size, NULL);

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
