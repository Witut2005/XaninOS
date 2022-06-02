
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

            static time_t now();
            static system_clock& get_instance();
            static time_t to_time_t(time_t);

            private:
            
            system_clock(){}

            static system_clock instance;

        };

        system_clock system_clock::instance;

time_t system_clock::now()
{
    asm("nop");        
}


system_clock& system_clock::get_instance()
{
    return system_clock::instance;
}






    }

}



