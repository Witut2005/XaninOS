#include <lib/stdiox.h>
#include <fs/ustar.h>

void pwd(void)
{
    xprintf("\r%s\n", get_current_directory());
    xscanf("%s",0x0);
}