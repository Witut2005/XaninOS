
#include "./time.hpp"
#include <lib/libc/memory.h>

namespace std
{

namespace chrono
{

CmosTime system_clock::now()
{
    time_get(&system_clock::time);
    return system_clock::time;
}

bcd system_clock::year()
{
    time_get(&system_clock::time);
    bcd x = { uint32_t(time.century * 0x100 + time.year) };
    return x;
}

std::string time_to_string(CmosTime x)
{
    char str[50];
    memset((uint8_t*)str, 0, sizeof(str));

    str[0] = x.hour / 0x10 + '0';
    str[1] = x.hour % 0x10 + '0';

    str[2] = ':';

    str[3] = x.minutes / 0x10 + '0';
    str[4] = x.minutes % 0x10 + '0';

    str[5] = ':';

    str[6] = x.seconds / 0x10 + '0';
    str[7] = x.seconds % 0x10 + '0';

    return std::string(str);
}

system_clock system_clock::instance;
CmosTime system_clock::time;

}
}