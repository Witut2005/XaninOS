
#pragma once




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

struct bcd
{
    uint32_t bcd_num;
};

extern "C"
{
    
void sleep(float seconds);
void msleep(float miliseconds);

}