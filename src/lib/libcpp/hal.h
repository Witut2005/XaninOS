
#pragma once

#include <lib/libc/hal.h>

namespace hal
{


class Port8 
{

    private:
    uint16_t port;

    explicit Port8(uint16_t port_number) : port(port_number){}
    Port8(const Port8& other) = default;

    void write(uint8_t data)
    {
        outbIO(this->port, data);
    }

    uint8_t read()
    {
        return inbIO(this->port);
    }

};

class Port16
{
    private:
    uint16_t port;

    public:
    explicit Port16(uint16_t port_number) : port(port_number){}
    Port16(const Port16& other) = default;
    
    void write(uint16_t data)
    {
        outwIO(this->port, data);
    }

    uint16_t read()
    {
        return inwIO(this->port);
    }

};

class Port32
{

    private:
    uint16_t port;

    public:
    explicit Port32(uint16_t port_number) : port(port_number){}
    Port32(const Port32& other) = default;
    
    void write(uint32_t data)
    {
        outdIO(this->port, data);
    }

    uint32_t read()
    {
        return indIO(this->port);
    }

};

}
