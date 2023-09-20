
#include "./string.h"

uint32_t strlen(const char* a)
{

    if(a == NULL)
        return 0;

    uint32_t length = 0;

    for(const char* i = a; *i != '\0' ;i++)
        length++;

    return length;
}


bool bstrcmp(char* a, const char* b)
{

    uint32_t lengtha = strlen(a);
    uint32_t lengthb = strlen(b);
    
    if(lengtha != lengthb)
        return 0;

    for(char* i = a; *i != '\0' ;i++)
    {
        if(*a != *b)
            return 0;
        a++;
        b++;
    }

    
    return 1;		
}

char* reverse_string(char* str)
{
    char buf;
    char* end = str + strlen(str) - 1;

    for(char* begin = str; (uint32_t)begin < (uint32_t)end; begin++, end--)
    {
        char buf = *begin;

        *begin = *end;
        *end = buf;
    }
    return str;
}

char* int_to_str(int x, char* buf)
{
    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    for(i = 0; x != 0; i++)
    {
        buf[i] = (x % 10) + '0';
        x = x / 10;
    }

    buf[i] = '\0';
    buf = reverse_string(buf);
    return buf;
}

char* int_to_hex_str(uint32_t x, char* buf)
{
    char hex_values[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

    int i = 0;

    if(!x)
    {
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = '\0';
        return buf;
    }
    
    for(i = 0; x != 0; i++)
    {
        buf[i] = hex_values[x % 16]; 
        x = x / 16;
    }

    buf = reverse_string(buf);
    return buf;
}
