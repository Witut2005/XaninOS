
#include <lib/libc/colors.h>
#include <lib/libc/endian.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <lib/system/system.h>
#include <sys/devices/pit/pit.h>

uint8_t null_region[20] = { 0 };

CmosTime SystemTime;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

char* daysLUT[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
uint8_t* null_memory_region;

char getchar(void)
{
    return __sys_getchar();
}

char getscan(void)
{
    return __sys_getscan();
}

xchar getxchar(void)
{
    return __sys_getxchar();
}

xchar inputg(void)
{
    return __sys_inputg();
}

bcd_date_t time_extern_date(CmosTime* Time)
{
    return ((uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)));
}

bcd_time_t time_extern_time(CmosTime* Time)
{
    return ((uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes));
}

CmosTime* time_get(CmosTime* Time)
{
    interrupt_disable();

    // GET SECONDS
    outbIO(CMOS_ADDR, 0x0);
    Time->seconds = inbIO(CMOS_DATA);

    // GET MINUTES
    outbIO(CMOS_ADDR, 0x2);
    Time->minutes = inbIO(CMOS_DATA);

    // GET HOURS
    outbIO(CMOS_ADDR, 0x4);
    Time->hour = inbIO(CMOS_DATA) + 1;

    // GET day
    outbIO(CMOS_ADDR, 0x6);
    Time->weekday = inbIO(CMOS_DATA);
    Time->weekday--;

    // GET DAY_OF_MONTH
    outbIO(CMOS_ADDR, 0x7);
    Time->day_of_month = inbIO(CMOS_DATA);

    // GET MONTH
    outbIO(CMOS_ADDR, 0x8);
    Time->month = inbIO(CMOS_DATA);

    // GET YEAR
    outbIO(CMOS_ADDR, 0x9);
    Time->year = inbIO(CMOS_DATA);

    // GET CENTURY
    outbIO(CMOS_ADDR, 0x32);
    Time->century = inbIO(CMOS_DATA);

    interrupt_enable();
    return Time;
}

bool app_exited = false;
