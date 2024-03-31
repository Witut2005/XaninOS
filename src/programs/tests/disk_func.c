
#include <sys/devices/hda/disk.h>
#include <sys/flow/exit_codes.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>

#define BYTES_TO_READ 15

__STATUS disk_func(void)
{
    char buf[16];
    buf[BYTES_TO_READ] = 0xFF;

    for (int i = 0; i < 16; i++)
        buf[i] = i;

    __disk_read_bytes(ATA_FIRST_BUS, ATA_MASTER, 0, 1, 15, (uint8_t*)buf);

    for (int i = 0; i < 16; i++)
        xprintf("%mx ", buf[i]);

    return XANIN_OK;
}