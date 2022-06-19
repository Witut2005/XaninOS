
#include <xin_fs/xin.c> 
#include <libc/string.h>
#include <libc/stdlibx.h>

void print_log(const char * str)
{

    static int current_line = 0;

    xin_entry* file = fopen("/syslog", "w");

    char buf[20] = {'\0'};

    char separator = ':';
    
    bcd_to_str(SystemTime.hour, buf);

    write(file, bcd_to_str(SystemTime.hour, buf), strlen(buf));

    write(file, &separator, 1);
    
    write(file, bcd_to_str(SystemTime.minutes, buf), strlen(buf));
    
    write(file, &separator, 1);
    write(file, bcd_to_str(SystemTime.seconds, buf), strlen(buf));

    
    separator = ' ';
    write(file, &separator, 1);


    write(file, str, strlen(str));


}