
#include <lib/stdiox.h>

void assert_func(bool condition, char* file_name, uint32_t line)
{
    if(!condition)
    {
        screen_clear();
        xprintf("assertion failed file: %s line: %d\n", file_name, line);
        while(1);
    }
}


#define assert(condition) assert_func(condition, __FILE__, __LINE__)