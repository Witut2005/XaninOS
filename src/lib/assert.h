
#include <lib/stdiox.h>

void assert(bool condition, char* file_name, uint32_t line)
{
    if(!condition)
    {
        screen_clear();
        xprintf("assertion failed file: %s line: %d\n", file_name, line);
        while(1);
    }

}