
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/system.h>
#include <fs/xin.h>

int c_test(void)
{
    XinEntry *Config = __xin_fopen("/etc/var/variables.conf", "r");

    // char *var = system_variable_get("HOME");
    // var = system_variable_get("USERNAME");
    // var = system_variable_get("HOSTNAME");
    char *var = getline(Config, 2);

    xprintf("TEST: %s\n", var);
    __xin_fclose(&Config);
    return XANIN_OK;
}