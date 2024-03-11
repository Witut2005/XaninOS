

#include <lib/libc/hal.h>
#include <stdint.h>
#include <sys/devices/com/com.hpp>

// void SerialPort::write(SerialPortRegisters reg, uint8_t value)
// {
//     return outbIO()
// }

// void SerialPort::red(SerialPortRegisters reg)
// {
//     return outbIO()
// }

using namespace Device;

static inline uint16_t operator+(uint16_t io_base, SerialPort::Register reg)
{
    return io_base + (uint16_t)reg;
}

bool SerialPort::probe(uint16_t io_base)
{
    outbIO(io_base + Register::InterruptEnable, false);
    outbIO(io_base + Register::LineControl, 0x80);
    outbIO(io_base + Register::DivisorLower, 0x3);
    outbIO(io_base + Register::DivisorUpper, 0);
    outbIO(io_base + Register::LineControl, 0x3);
}

SerialPort &SerialPort::create(uint16_t) {}

void SerialPortManager::initialize(void)
{
    for (int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
    {
        m_ports[i] = &SerialPort::create(SerialPort::s_addresses[i]);
    }
}
