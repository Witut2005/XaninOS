

#include <lib/libc/hal.h>
#include <stdint.h>
#include <sys/devices/com/com.hpp>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <lib/libc/loop.h>
#include <lib/libcpp/algorithm.h>
#include <sys/pmmngr/alloc.h>

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
    return inbIO(m_iobase + reg);
}

void SerialPort::write(Register reg, uint8_t val) const
{
    outbIO(m_iobase + reg, val);
}

SerialPortManager& SerialPortManager::instance_get(void)
{
    return s_instance;
}

bool SerialPortManager::is_initialized()
{
    return s_initialized;
}

bool SerialPortManager::initialize(uint16_t default_divisor)
{
    if (mmngr_is_initialized() == false)
        return false;
    if (s_initialized)
        return is_functional();

    ITERATE_OVER(i, s_max_amount_of_ports)
    {
        s_ports[i] = SerialPort::try_to_create(s_addresses[i], default_divisor);
    }

    s_initialized = true;
    return is_functional();
}

uint8_t SerialPortManager::receive(void) {}

void SerialPortManager::send(uint8_t val)
{
    auto port = first_valid_port_get();
    if (port != nullptr)
        port->write(SerialPort::Register::Data, val);
}

SerialPort* SerialPortManager::first_valid_port_get(void)
{
    return *std::FindFirst<SerialPort**>::call(s_ports, &s_ports[s_max_amount_of_ports], [](SerialPort** ptr) { return (*ptr) != nullptr; });
}

bool SerialPortManager::is_functional(void)
{
    return is_initialized() && first_valid_port_get() != nullptr;
}

SerialPort* SerialPortManager::s_ports[s_max_amount_of_ports];
bool SerialPortManager::s_initialized;
SerialPortManager SerialPortManager::s_instance;

extern "C"
{
    bool serial_port_initialize(uint16_t default_divisor)
    {
        return SerialPortManager::initialize(default_divisor);
    }

    void serial_port_byte_send(uint8_t val)
    {
        return SerialPortManager::send(val);
    }

    void serial_port_array_send(uint8_t* arr, uint32_t size)
    {
        for (int i = 0; i < size; i++)
            SerialPortManager::send(arr[i]);
    }

    void serial_port_string_send(const char* str)
    {
        uint32_t len = strlen(str);
        for (int i = 0; i < len; i++)
            SerialPortManager::send(str[i]);
    }

    void dbg_success(const char* label, const char* msg)
    {
        if (SerialPortManager::is_functional() == false)
            return;

        serial_port_string_send("\033[32m[");
        serial_port_string_send(label);
        serial_port_string_send("] ");
        serial_port_string_send("\033[0m");
        serial_port_string_send(msg);
        serial_port_byte_send('\n');
    }

    void dbg_info(const char* label, const char* msg)
    {
        if (SerialPortManager::is_functional() == false)
            return;

        serial_port_string_send("\033[94m[");
        serial_port_string_send(label);
        serial_port_string_send("] ");
        serial_port_string_send("\033[0m");
        serial_port_string_send(msg);
        serial_port_byte_send('\n');
    }

    void dbg_warning(const char* label, const char* msg)
    {
        if (SerialPortManager::is_functional() == false)
            return;

        serial_port_string_send("\033[33m[");
        serial_port_string_send(label);
        serial_port_string_send("] ");
        serial_port_string_send("\033[0m");
        serial_port_string_send(msg);
        serial_port_byte_send('\n');
    }

    void dbg_error(const char* label, const char* msg)
    {
        if (SerialPortManager::is_functional() == false)
            return;

        serial_port_string_send("\033[31m[");
        serial_port_string_send(label);
        serial_port_string_send("] ");
        serial_port_string_send("\033[0m");
        serial_port_string_send(msg);
        serial_port_byte_send('\n');
    }
}
