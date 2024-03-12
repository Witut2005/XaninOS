

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
    constexpr uint8_t test_byte = 0x30;

    outbIO(io_base + Register::InterruptEnable, false);
    outbIO(io_base + Register::LineControl, 0x80); // enable dlab
    outbIO(io_base + Register::DivisorLower, 0x3);
    outbIO(io_base + Register::DivisorUpper, 0);
    outbIO(io_base + Register::LineControl, DataBits::Bit8);
    outbIO(io_base + Register::Fifo, 0xC7); // enable fifo 
    outbIO(io_base + Register::ModemControl, DataTerminalReady | RequestToSend | Out2);
    outbIO(io_base + Register::ModemControl, RequestToSend | Out1 | Out2 | Loopback);
    outbIO(io_base + Register::Data, test_byte);

    return inbIO(io_base) == test_byte ? [&]() -> bool {outbIO(io_base + Register::ModemControl, 0xF); return true;}() : false;
}

SerialPort &SerialPort::create(uint16_t) {}

bool SerialPortManager::initialize(void)
{
    for (int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
    {
        if(SerialPort::probe(SerialPort::s_addresses[i]))
            return true;
    }
    return false;
}

extern "C"  {
    bool serial_port_initialize(void)
    {
        return SerialPortManager::initialize();
    }
}