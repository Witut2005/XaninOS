

#include <lib/libc/hal.h>
#include <stdint.h>
#include <sys/devices/com/com.hpp>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>

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

SerialPort::SerialPort(uint16_t io_base) : io_base(io_base) {}

uint8_t SerialPort::read(Register reg)
{
    return inbIO(this->io_base + reg);
}

void SerialPort::write(Register reg, uint8_t val)
{
    outbIO(this->io_base + reg, val);
}

SerialPort* SerialPort::try_to_create(uint16_t io_base)
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

    outbIO(io_base + Register::ModemControl, 0xF);

    return inbIO(io_base) == test_byte ? new SerialPort(io_base) : nullptr;
}

bool SerialPortManager::is_initialized() 
{
    for(int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
        if(SerialPortManager::ports[i] != nullptr) return true;
    return false;
}

bool SerialPortManager::initialize(void)
{
    for (int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
        SerialPortManager::ports[i] = SerialPort::try_to_create(SerialPort::s_addresses[i]);
    return SerialPortManager::is_initialized();
}

uint8_t SerialPortManager::receive(void){}

bool SerialPortManager::send(uint8_t val) 
{
    for (int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
    {
        if(SerialPortManager::ports[i] != nullptr)
        {
            // SerialPortManager::ports[i]->write(SerialPort::Register::Data, val);
            outbIO(0x3F8, val);
            return true;
        }
    }
    return false;
}

SerialPort* SerialPortManager::ports[SerialPort::s_max_amount_of_ports];

extern "C"  {
    bool serial_port_initialize(void)
    {
        return SerialPortManager::initialize();
    }

    bool serial_port_byte_send(uint8_t val)
    {
        return SerialPortManager::send(val);
    }

    void serial_port_array_send(uint8_t* arr, uint32_t size)
    {
        for(int i = 0; i < size; i++)
            SerialPortManager::send(arr[i]);
    }

    void serial_port_string_send(char* str)
    {
        uint32_t len = strlen(str);
        for(int i = 0; i < len; i++)
            SerialPortManager::send(str[i]);
    }
}