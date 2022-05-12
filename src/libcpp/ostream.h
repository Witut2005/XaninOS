
#include <stdint.h>
#include <libcpp/cstdio.h>

namespace std
{


struct ostream_options
{
    uint8_t option;
};


ostream_options endl = {0};

class ostream 
{

    public:
    /*
    cout(uint32_t x)
    {
       xprintf("%d", x); 
    }
    */

    void operator<<(ostream_options x)
    {
        xprintf("\n");
    }

    void operator<<(uint32_t x)
    {
        xprintf("%d", x); 
    }

    void operator<<(int x)
    {
        xprintf("%d", x); 
    }

    void operator<<(char x)
    {
        xprintf("%c",x);
    }

    void operator<<(uint16_t x)
    {
        xprintf("%d",x);
    }

}cout;


}
