
#include <stdint.h>
#include <libc/stdlibx.h>
#include <libc/time.h>
// #include <libc/hal.h>

uint32_t cpu_mhz = 0x0;
uint32_t cpu_khz = 0x0;


extern float pit_time;

void sleep(float seconds)
{
    float now = pit_time;

    while(pit_time < now + seconds);

}

void msleep(float miliseconds)
{

    miliseconds = miliseconds / 1000;

    float now = pit_time;

    while(pit_time < now + miliseconds);

}

time_offset_t* start(void)
{
    time_offset_t* time = (time_offset_t*)calloc(sizeof(time_offset_t));
    *time = (time_offset_t)pit_time;
    return time;
}

void stop(time_offset_t* time)
{
    time_offset_t tmp = pit_time;
    *time = pit_time - *time;
}

// void msleep_