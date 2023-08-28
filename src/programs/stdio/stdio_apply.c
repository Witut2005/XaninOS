
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>

uint32_t stdio_refresh_rate;
interval_id stdio_refresh_interval_id;

__STATUS stdio_apply(void)
{
    char* buffer = (char*)calloc(100 * SIZE_OF(char));

    XinEntry* StdioRefreshRateConfig = fopen("/etc/stdio/refresh_rate.conf", "r");
    fseek(StdioRefreshRateConfig, ARRAY_LENGTH("STDIO_REFRESH_RATE: ") - 1);

    fread(StdioRefreshRateConfig, buffer, 99);

    stdio_refresh_rate = strtoi(buffer, 10);

    interval_clear(stdio_refresh_interval_id);

    if(stdio_refresh_rate <= 100)
        xprintf("Stdio new refresh rate: %d\n", stdio_refresh_rate);

    else
    {
        puts_error("Stdio refresh rate cannot be more than 100ms\n");
        stdio_refresh_rate = 100;
    }

    interval_set(stdio_refresh, stdio_refresh_rate, NULL);

}