
#pragma once

#include <libcpp/chal.h>


class port8
{
    uint16_t port;

    public:
    port8(uint16_t port_number) : port(port_number){}
    void write(uint8_t data)
    {
        asm("outb dx,al" :: "d" (this->port), "a"(data));
    }

    uint8_t read()
    {
        return inbIO(this->port);
    }

};

class port16
{
    uint16_t port;

    public:
    port16(uint16_t port_number) : port(port_number){}
    
    void write(uint16_t data)
    {
        asm("out dx,ax" :: "d" (this->port), "a"(data));
    }

    uint16_t read()
    {
        return inwIO(this->port);
    }

};

class port32
{

    uint16_t port;

    public:
    port32(uint16_t port_number) : port(port_number){}
    
    void write(uint32_t data)
    {
        asm("out dx,eax" :: "d" (this->port), "a"(data));
    }

    uint32_t read()
    {
        return indIO(this->port);
    }

};
