
#include <lib/libc/hal.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <stdint.h>

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

CmosTime SystemTime;
char* daysLUT[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
float pit_time;

void sleep(float seconds)
{
    float now = pit_time;

    while (pit_time < now + seconds)
        ;
}

void msleep(float miliseconds)
{
    miliseconds = miliseconds / 1000;
    // pit_time = 0;
    float now = pit_time;

    while (pit_time < now + miliseconds)
        ;
}

time_offset_t* start(void)
{
    time_offset_t* time = (time_offset_t*)calloc(sizeof(time_offset_t));
    *time = (time_offset_t)pit_time;
    return time;
}

void stop(time_offset_t* time)
{
    *time = pit_time - *time;
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
