
#pragma once

#include <stdint.h>
#include <lib/libc/time.h>
#include <lib/libcpp/string.h>


namespace std
{

    namespace chrono
    {
    
        class system_clock
        {
    
            public:
                
            system_clock(const system_clock&) = delete;   //copy constructor

            static CmosTime now();
            static bcd year();
            static system_clock& get_instance();
            static time_t to_time_t(time_t);


            private:
            
            system_clock(){}

            static CmosTime time;
            static system_clock instance;

        };

    inline system_clock system_clock::instance;
    inline CmosTime system_clock::time;

    inline CmosTime system_clock::now()
    {
        time_get(&system_clock::time);       
        return system_clock::time;
    }

    inline bcd system_clock::year()
    {
        time_get(&system_clock::time);
        bcd x = {uint32_t(time.century * 0x100 + time.year)};
        return x;
    }


    inline system_clock& system_clock::get_instance()
    {
        return system_clock::instance;
    }


    static inline std::string time_to_string(CmosTime x)
    {
        char str[9] = {'\0'};

        str[0] = x.hour / 0x10 + '0';
        str[1] = x.hour % 0x10 + '0';

        str[2] = ':';

        str[3] = x.minutes / 0x10 + '0';
        str[4] = x.minutes % 0x10 + '0';

        str[5] = ':';

        str[6] = x.seconds / 0x10 + '0';
        str[7] = x.seconds % 0x10 + '0';
    
        std::string ret = str;
        return ret;

    }




    }

}



