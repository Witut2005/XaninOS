
#include <stdint.h>
#include <libc/stdlibx.h>
// #include <libc/hal.h>

uint32_t cpu_mhz = 0x0;
uint32_t cpu_khz = 0x0;

struct time_t
{

    uint32_t time_t_sec : 6;
    uint32_t time_t_min : 6;
    uint32_t time_t_hs  : 5;
    uint32_t time_t_mday: 5;
    uint32_t time_t_mon : 4;
    uint32_t time_t_year: 4;
    uint32_t time_t_wday: 3;
    uint32_t time_t_yday: 9;
    uint32_t time_t_isdst;
    
};

typedef struct time_t time_t;

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


// void msleep_