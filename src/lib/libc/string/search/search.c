
#include "../info/info.h"
#include <stddef.h>
#include <stdint.h>

enum CHAR_FIND_OPTIONS {
    CHAR_FIND_DIGITS = 0xFE,
    CHAR_FIND_LETTERS = 0xFF
};

char* char_find(char* str, char c)
{
    if (c == CHAR_FIND_LETTERS)
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))
            {
                return &str[i];
            }
        }
    }

    else if (c == CHAR_FIND_DIGITS)
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] >= '0' && str[i] <= '9')
            {
                return &str[i];
            }
        }
    }

    else
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] == c)
            {
                return &str[i];
            }
        }
    }

    return NULL;
}

char* char_find_from_end(char* str, uint32_t offset, char c)
{
    char* it = str + (strlen(str) - offset - 1);

    if (c == CHAR_FIND_LETTERS)
    {
        for (; it >= str; it--)
        {
            if ((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z'))
            {
                return it;
            }
        }
    }

    else if (c == CHAR_FIND_DIGITS)
    {
        for (; it >= str; it--)
        {
            if (*it >= '0' && *it <= '9')
            {
                return it;
            }
        }
    }

    else
    {
        for (; it >= str; it--)
        {
            if (*it == c)
            {
                return it;
            }
        }
    }

    return NULL;
}
