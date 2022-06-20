
#include <xin_fs/xin.c> 
#include <libc/string.h>
#include <libc/stdlibx.h>

void print_log(const char * str)
{

    static int current_line;

    xin_entry* file = fopen("/syslog", "w");

    char buf[80] = {' '};

    if(current_line > 27)
    {
        fseek(file, 0);

        for(int i = 0; i < 28; i++)
            write(file, buf , 80);
        current_line = 0;
    }

    fseek(file, current_line * 80);

    buf[79] = '\0';

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

    current_line++;

}
