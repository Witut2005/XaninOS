
#include <fs/xin.h>
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>
#include <lib/libc/file.h>

#define PRINTK_STRING_MAX_LENGTH 70

bool syslog_enabled;

void syslog_disable(void)
{
    syslog_enabled = false;
}

void syslog_enable(void)
{
    syslog_enabled = true;
}

bool is_syslog_enabled(void)
{
    return syslog_enabled;
}

void printk(const char *str)
{

    if (!is_syslog_enabled())
        return;

    char buf[4] = {0};

    XinEntry *file = fopen("/syslog", "rw");

    fseek(file, file->size);

    fwrite(file, bcd_to_str(SystemTime.hour, buf), 2);

    fwrite(file, ":", 1);

    fwrite(file, bcd_to_str(SystemTime.minutes, buf), 2);

    fwrite(file, ":", 1);
    fwrite(file, bcd_to_str(SystemTime.seconds, buf), 2);

    fwrite(file, " ", 1);

    fwrite(file, (void *)str, strlen(str) < PRINTK_STRING_MAX_LENGTH ? strlen(str) : PRINTK_STRING_MAX_LENGTH);

    fwrite(file, "\n", 1);

    fclose(&file);
}
