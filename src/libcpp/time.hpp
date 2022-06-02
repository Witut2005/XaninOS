#include <stdint.h>

struct tm
{

    uint32_t tm_sec : 6;
    uint32_t tm_min : 6;
    uint32_t tm_hs  : 5;
    uint32_t tm_mday: 5;
    uint32_t tm_mon : 4;
    uint32_t tm_year: 4;
    uint32_t tm_wday: 3;
    uint32_t tm_yday: 9;
    uint32_t tm_isdst;
    
};

typedef struct tm tm;

namespace std
{

    namespace chrono
    {
    
        class system_clock
        {
    
            public:
                
            static tm now()
            {
                asm("nop");        
            }

            static system_clock& get_instance()
            {
                return instance;
            }

            static system_clock instance;

        }SystemClock;

        system_clock system_clock::instance;

    }

}
