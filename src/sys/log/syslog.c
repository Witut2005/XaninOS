
#include <fs/xin.h> 
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>
// #include <lib/libc/stdlibx.h>
// #include <sys/terminal/vty/vty.h>

// static uint32_t current_line;

void printk(const char * str)
{
    return;

    interrupt_disable();

    XinEntry* file = fopen("/syslog", "w");

    char buf[VGA_WIDTH] = {0};

    char separator = ':';
    fseek(file, file->size);
    
    fwrite(file, bcd_to_str(SystemTime.hour, buf), 2);

    fwrite(file, &separator, 1);
    
    fwrite(file, bcd_to_str(SystemTime.minutes, buf), 2);
    
    fwrite(file, &separator, 1);
    fwrite(file, bcd_to_str(SystemTime.seconds, buf), 2);
    
    separator = ' ';
    fwrite(file, &separator, 1);

    fwrite(file, (void*)str, strlen(str));

    fwrite(file, "\n", 2);


    // current_line++;

    fclose(&file);
    interrupt_enable();
}
