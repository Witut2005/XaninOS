
#include "../local_macros.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_in_char_range(char r1, char r2, char c)
{
    return c > r1 && c < r2;
}

bool is_char(char c)
{
    return c > '0' && c < '~';
}

uint32_t strlen(char const* str)
{
    LIBC_STR_EXIT_ON_EQUALS_ZERO(str, 0);

    uint32_t length = 0;
    for (int i = 0; str[i] != '\0'; i++, length++)
        ;

    return length;
}

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

uint32_t number_of_lines_get(char const* str)
{
    uint32_t lines = 0;

    while (*str)
    {
        if (*str == '\n')
        {
            lines++;
        }
        str++;
    }
    return lines + 1;
}

uint32_t size_of_biggest_line_get(char const* str)
{
    int max_line_size = 0;
    int current_line_size = 0;

    while (*str)
    {
        if (*str == '\n') // do not include \x1e
        {
            if (current_line_size > max_line_size)
            {
                max_line_size = current_line_size;
            }

            current_line_size = 0;
        } else
        {
            current_line_size++;
        }
        str++;
    }

    if (current_line_size > max_line_size)
    {
        max_line_size = current_line_size;
    }

    return max_line_size;
}
