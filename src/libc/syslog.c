
#include <xin_fs/xin.h> 
#include <libc/string.h>
#include <libc/stdlibx.h>
#include <libc/stdiox.h>
#include <terminal/vty.h>
#include <keyboard/scan_codes.h>
#include <libc/hal.h>

// static uint32_t current_line;

void printk(const char * str)
{

    interrupt_disable();

    XinEntry* file = fopen("/syslog", "w");

    char* buf = calloc(80);


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

    fwrite(file, "\n", 1);


    // current_line++;
    free(buf);

    fclose(&file);
    interrupt_enable();
}
