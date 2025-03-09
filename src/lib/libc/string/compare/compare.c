
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int32_t strcmp(char const* a, char const* b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return *a - *b;
}

bool bstrcmp(char const* a, char const* b)
{
    return strcmp(a, b) == 0;
}

bool bstrncmp(char const* a, char const* b, size_t string_size)
{
    // uint32_t length_to_check = strlen()
    for (int i = 0; i < string_size; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}
