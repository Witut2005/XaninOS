
#include <sys/log/syslog.h>
#include <lib/libc/stdlibx.h>

int print_to_syslog(char* str)
{
    printk(str);
    return XANIN_OK;
}