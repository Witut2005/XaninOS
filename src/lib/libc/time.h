
#pragma once

#include <stdint.h>

struct CmosTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekday;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} __attribute__((packed));

typedef struct CmosTime CmosTime;

typedef uint32_t bcd_date_t;
typedef uint16_t bcd_time_t;

struct time_t {

    uint32_t time_t_sec : 6;
    uint32_t time_t_min : 6;
    uint32_t time_t_hs : 5;
    uint32_t time_t_mday : 5;
    uint32_t time_t_mon : 4;
    uint32_t time_t_year : 4;
    uint32_t time_t_wday : 3;
    uint32_t time_t_yday : 9;
    uint32_t time_t_isdst;
};

struct bcd {
    uint32_t bcd_num;
};

#ifndef __cplusplus
typedef struct time_t time_t;
typedef struct bcd bcd;
#endif

typedef uint32_t time_offset_t;

extern CmosTime SystemTime;
extern char* daysLUT[7];
extern float pit_time;

#ifdef __cplusplus
extern "C" {
#endif

    void sleep(float seconds);
    void msleep(float miliseconds);
    time_offset_t* start(void);
    void stop(time_offset_t* time);
    bcd_date_t time_extern_date(CmosTime* Time);
    bcd_time_t time_extern_time(CmosTime* Time);
    CmosTime* time_get(CmosTime* Time);

#ifdef __cplusplus
}
#endif
