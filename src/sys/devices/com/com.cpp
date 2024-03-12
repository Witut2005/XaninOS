

#include <lib/libc/hal.h>
#include <stdint.h>
#include <sys/devices/com/com.hpp>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <lib/libc/loop.h>

using namespace Device;

static inline uint16_t operator+(uint16_t iobase, SerialPort::Register reg)
{
    return iobase + (uint16_t)reg;
}

SerialPort* SerialPort::try_to_create(uint16_t iobase, uint16_t divisor)
{
    constexpr uint8_t test_byte = 0x30;

    outbIO(iobase + Register::InterruptEnable, false);
    outbIO(iobase + Register::LineControl, DlabEnable);
    outbIO(iobase + Register::DivisorLower, divisor & 0xFF);
    outbIO(iobase + Register::DivisorUpper, divisor >> 8);
    outbIO(iobase + Register::LineControl, DataBits::Bit8);
    outbIO(iobase + Register::Fifo, 0xC7); // enable fifo 
    outbIO(iobase + Register::ModemControl, DataTerminalReady | RequestToSend | Out2);
    outbIO(iobase + Register::ModemControl, RequestToSend | Out1 | Out2 | Loopback);
    outbIO(iobase + Register::Data, test_byte);

    outbIO(iobase + Register::ModemControl, 0xF);

    return inbIO(iobase) == test_byte ? new SerialPort(iobase, divisor) : nullptr;
}

SerialPort::SerialPort(uint16_t iobase, uint16_t divisor) : m_iobase(iobase), m_divisor(divisor) {}

uint8_t SerialPort::read(Register reg) const
{
    return inbIO(this->m_iobase + reg);
}

void SerialPort::write(Register reg, uint8_t val) const
{
    outbIO(this->m_iobase + reg, val);
}

bool SerialPortManager::is_initialized() 
{
    for(int i = 0; i < SerialPort::s_max_amount_of_ports; i++)
        if(SerialPortManager::s_ports[i] != nullptr) return true;
    return false;
}

bool SerialPortManager::initialize(uint16_t default_divisor)
{
    ITERATE_OVER(i, SerialPort::s_max_amount_of_ports) {
        SerialPortManager::s_ports[i] = SerialPort::try_to_create(SerialPort::s_addresses[i], default_divisor);
    }
    return SerialPortManager::is_initialized();
}

uint8_t SerialPortManager::receive(void){}

bool SerialPortManager::send(uint8_t val) 
{
    ITERATE_OVER(i, SerialPort::s_max_amount_of_ports)
    {
        if(SerialPortManager::s_ports[i] != nullptr)
        {
            SerialPortManager::s_ports[i]->write(SerialPort::Register::Data, val);
            return true;
        }
    }
    return false;
}

SerialPort* SerialPortManager::s_ports[SerialPort::s_max_amount_of_ports];

extern "C"  {
    bool serial_port_initialize(uint16_t default_divisor)
    {
        return SerialPortManager::initialize(default_divisor);
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