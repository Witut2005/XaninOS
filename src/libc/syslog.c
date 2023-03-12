
#include <xin_fs/xin.h> 
#include <libc/string.h>
#include <libc/stdlibx.h>
#include <libc/stdiox.h>
#include <terminal/vty.h>
#include <keyboard/scan_codes.h>

// static uint32_t current_line;

void printk(const char * str)
{

    XinEntry* file = fopen("/syslog", "rw");

    char* buf = calloc(80);


    char separator = ':';
    fseek(file, file->size);
    
    fwrite(file, bcd_to_str(SystemTime.hour, buf), strlen(buf));

    fwrite(file, bcd_to_str(SystemTime.hour, buf), strlen(buf));

    fwrite(file, &separator, 1);
    
    fwrite(file, bcd_to_str(SystemTime.minutes, buf), strlen(buf));
    
    fwrite(file, &separator, 1);
    fwrite(file, bcd_to_str(SystemTime.seconds, buf), strlen(buf));

    
    separator = ' ';
    fwrite(file, &separator, 1);


    fwrite(file, (void*)str, strlen(str));
    fwrite(file, "\n", 1);

    // current_line++;
    free(buf);

    fclose(&file);
}
