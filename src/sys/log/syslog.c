
#include <fs/xin.h> 
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>

#define PRINTK_STRING_MAX_LENGTH 70

void printk(const char * str)
{

    char buf[4] = {0};

    XinEntry* file = fopen("/syslog", "rw");

    fseek(file, file->size);
    
    fwrite(file, bcd_to_str(SystemTime.hour, buf), 2);

    fwrite(file, ":", 1);
    
    fwrite(file, bcd_to_str(SystemTime.minutes, buf), 2);
    
    fwrite(file, ":", 1);
    fwrite(file, bcd_to_str(SystemTime.seconds, buf), 2);
    
    fwrite(file, " ", 1);

    fwrite(file, (void*)str, strlen(str) < PRINTK_STRING_MAX_LENGTH ? strlen(str) : PRINTK_STRING_MAX_LENGTH);

    fwrite(file, "\n", 1);

    fclose(&file);
}

