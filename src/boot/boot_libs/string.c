
#include "./string.h"

uint32_t strlen(const char* a)
{

    if (a == NULL)
        return 0;

    uint32_t length = 0;

    for (const char* i = a; *i != '\0'; i++)
        length++;

    return length;
}

bool bstrcmp(const char* a, const char* b)
{

    uint32_t lengtha = strlen(a);
    uint32_t lengthb = strlen(b);

    if (lengtha != lengthb)
        return 0;

    while (*a != '\0')
    {
        if (*a != *b)
            return 0;
        a++;
        b++;
    }

    return 1;
}

char* strrev(char* str)
{
    char* end = str + strlen(str) - 1;

    for (char* begin = str; (uint32_t)begin < (uint32_t)end; begin++, end--)
    {
        char buf = *begin;

        *begin = *end;
        *end = buf;
    }
    return str;
}
char* int_to_string(uint32_t value, char* buf, const uint8_t base)
{
    char digits[] = { '0','1','2','3','4','5','6','7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                        't', 'u', 'v', 'w' };

    if (base > 32)
        return NULL;

    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    int i = 0;
    while (value) {
        buf[i++] = digits[value % base];
        value = value / base;
    };
    buf[i] = '\0';

    strrev(buf);
    return buf;
}