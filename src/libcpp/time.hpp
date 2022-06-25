
#pragma once

#include <stdint.h>
#include <libcpp/ctime.h>




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

    system_clock system_clock::instance;
    CmosTime system_clock::time;

    CmosTime system_clock::now()
    {
        time_get(&system_clock::time);       
        return system_clock::time;
    }

    bcd system_clock::year()
    {
        time_get(&system_clock::time);
        bcd x = {uint32_t(time.century * 0x100 + time.year)};
        return x;
    }


    system_clock& system_clock::get_instance()
    {
        return system_clock::instance;
    }






    }

}



