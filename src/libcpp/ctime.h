
#pragma once

struct CmosTime
{
    uint8_t seconds; 
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekday;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t century;
    

}__attribute__((packed));


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

extern "C"
{
    
void sleep(float seconds);
void msleep(float miliseconds);

}