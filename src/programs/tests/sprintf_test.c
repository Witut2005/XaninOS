#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/com/com.h>

__STATUS sprintf_test(void)
{
    char* buf = (char*)calloc(50);

    puts("Results will be emitted to Serial Port Device");

    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test: %x", 0xabcd));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test:%u", 123));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test:%d", -10));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test:%10c", 'o'));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test:%06s", "ble"));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "xsprintf test:%6d", -1));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "bcd date test:%y", 0x03272024));
    dbg_success(DEBUG_LABEL_LIBC, xsprintf(buf, "bcd time test:%t", 0x2130));

    free(buf);

}