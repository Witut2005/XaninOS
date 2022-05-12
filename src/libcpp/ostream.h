
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

    ostream& operator<<(ostream_options x)
    {
        if(!x.option)
            xprintf("\n");
    }

    ostream& operator<<(unsigned int x)
    {
        xprintf("%d", x); 
    }

    ostream& operator<<(int x)
    {
        xprintf("%d", x); 
    }

    ostream& operator<<(char x)
    {
        xprintf("%c",x);
    }

    ostream& operator<<(unsigned char x)
    {
        xprintf("%c",x);
    }

    ostream& operator<<(unsigned short x)
    {
        xprintf("%d",x);
    }

    ostream& operator<<(short x)
    {
        xprintf("%d",x);
    }

}cout;


}
