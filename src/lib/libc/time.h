
#pragma once

#include <stdint.h>

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

struct bcd
{
    uint32_t bcd_num;
};

#ifndef __cplusplus
typedef struct time_t time_t;
typedef struct bcd bcd;
#endif

typedef uint32_t time_offset_t;

extern uint32_t cpu_mhz;
extern uint32_t cpu_khz;

#ifdef __cplusplus
extern "C"{
#endif

    void sleep(float seconds);
    void msleep(float miliseconds);
    time_offset_t* start(void);
    void stop(time_offset_t* time);

#ifdef __cplusplus
}
#endif