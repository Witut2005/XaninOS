
#pragma once

#include <lib/libc/hal.h>

namespace hal{


class Port8 
{

    public:
    uint16_t port;

    explicit Port8(uint16_t port_number) : port(port_number){}
    void write(uint8_t data)
    {
        outbIO(this->port, data);
        //asm("outb dx,al" :: "d" (this->port), "a"(data));
    }

    uint8_t read()
    {
        return inbIO(this->port);
    }

};

class Port16
{
    public:
    uint16_t port;

    public:
    explicit Port16(uint16_t port_number) : port(port_number){}
    
    void write(uint16_t data)
    {
        outwIO(this->port, data);
        //asm("out dx,ax" :: "d" (this->port), "a"(data));
    }

    uint16_t read()
    {
        return inwIO(this->port);
        //return inwIO(this->port);
    }

};

class Port32
{

    uint16_t port;

    public:
    explicit Port32(uint16_t port_number) : port(port_number){}
    
    void write(uint32_t data)
    {
        outdIO(this->port, data);
        //asm("out dx,eax" :: "d" (this->port), "a"(data));
    }

    uint32_t read()
    {
        return indIO(this->port);
        //return indIO(this->port);
    }

};

}
