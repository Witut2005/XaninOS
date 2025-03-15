#include "./interval.h"

extern float pit_time;

IntervalEntry XaninIntervals[INTERVALS_MAX] = { INTERVAL_CLEAR };

interval_id interval_set(interval_handler handler, float ms, address_t* args)
{

    for (int i = 0; i < INTERVALS_MAX; i++)
    {
        if (!XaninIntervals[i].is_in_use)
        {
            XaninIntervals[i].is_in_use = INTERVAL_IN_USE;
            XaninIntervals[i].handler = handler;
            XaninIntervals[i].arguments = args;
            XaninIntervals[i].timeout = ms / 1000;
            XaninIntervals[i].start_time = 0;
            return (interval_id)i;
        }
    }
    return INTERVAL_CANT_INIT;
}

void interval_clear(interval_id used_interval)
{
    XaninIntervals[used_interval].is_in_use = INTERVAL_CLEAR;
}

void all_intervals_clear(void)
{
    for (int i = 0; i < INTERVALS_MAX; i++)
        XaninIntervals[i].is_in_use = INTERVAL_CLEAR;
}

void do_interval(interval_id interval)
{
    if (XaninIntervals[interval].is_in_use)
    {
        if (pit_time > (XaninIntervals[interval].start_time + XaninIntervals[interval].timeout)) // timeout reached
        {
            XaninIntervals[interval].start_time = pit_time;
            XaninIntervals[interval].handler(XaninIntervals[interval].arguments);
        }
    }
}
