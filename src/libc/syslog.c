
#include <xin_fs/xin.h> 
#include <libc/string.h>
#include <libc/stdlibx.h>
#include <libc/stdiox.h>
#include <terminal/vty.h>
#include <keyboard/scan_codes.h>

// static uint32_t current_line;

void printk(const char * str)
{

    xin_entry* file = fopen("/syslog", "rw");

    char* buf = calloc(80);


    char separator = ':';
    
    write(file, bcd_to_str(SystemTime.hour, buf), strlen(buf));

    write(file, bcd_to_str(SystemTime.hour, buf), strlen(buf));

    write(file, &separator, 1);
    
    write(file, bcd_to_str(SystemTime.minutes, buf), strlen(buf));
    
    write(file, &separator, 1);
    write(file, bcd_to_str(SystemTime.seconds, buf), strlen(buf));

    
    separator = ' ';
    write(file, &separator, 1);


    write(file, str, strlen(str));
    write(file, "\n", 1);

    // current_line++;
    free(buf);

    // screen_clear();
    // xprintf("%s", file->file_info->base_address_memory);
    fseek(file, 0);
    // while(KeyInfo.scan_code != ENTER);

    fclose(&file);
}
