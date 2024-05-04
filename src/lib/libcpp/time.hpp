
#pragma once

#include <stdint.h>
#include <lib/libc/time.h>
#include <lib/libc/stdlibx.h>

namespace std
{

namespace chrono
{

class system_clock
{
private:
    system_clock() {};
    system_clock(const system_clock&) = delete;   //copy constructor

    static CmosTime time;
    static system_clock instance;

public:

    static CmosTime now();
    static bcd year();
    static time_t to_time_t(time_t);
    static inline system_clock& get_instance() { return instance; }
};

std::string time_to_string(CmosTime x);

}

}



