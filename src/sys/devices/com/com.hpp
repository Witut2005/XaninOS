
#pragma once

#include <stdint.h>
#include <lib/libcpp/class.hpp>
#include <sys/devices/com/com.h>

// first define all public types,
// then define all non-public types,
// then declare all public constructors,
// then declare the public destructor,
// then declare all public member functions,
// then declare all public member variables,
// then declare all non-public constructors,
// then declare the non-public destructor,
// then declare all non-public member functions, and
// then declare all non-public member variables.


namespace Device
{
class SerialPort
{


public:

    enum class Register : int8_t
    {
        // DLAB = 0
        Data = 0,
        InterruptEnable = 1,

        // DLAB = 1
        DivisorLower = 0,
        DivisorUpper = 1,

        InterruptIdentification = 2,
        Fifo = 2,

        LineControl,
        ModemControl,
        LineStatus,
        ModemStatus,
        ScratchRegister
    };

    enum Dlab : uint8_t 
    {
        DlabEnable = 1 << 7
    };

    enum DataBits : uint8_t
    {
        Bit5,
        Bit6,
        Bit7,
        Bit8
    };

    enum StopBist : uint8_t
    {
        bit1,
        bits2 = 1 << 1,
    };

    enum Parity : uint8_t
    {
        None = 0,
        Odd = 1 << 3,
        Even = 3 << 3,
        Mark = 5 << 3,
        Space = 7 << 3
    };

    enum InterruptEnable : uint8_t
    {
        DataAvailable = 1,
        TransmitterEmpty = 1 << 1,
        BreakOrError = 1 << 2,
        StatusChange = 1 << 3,
    };

    enum ModemControl : uint8_t 
    {
        DataTerminalReady = 1,
        RequestToSend = 1 << 1,
        Out1 = 1 << 2,
        Out2 = 1 << 3,
        Loopback = 1 << 4,
    };

    enum LineStatus : uint8_t
    {
        DataReady = 1,
        OverrunError = 1 << 1,
        ParityError = 1 << 2,
        FramingError = 1 << 3,
        BreakIndicator = 1 << 4,
        TransmitterHolderRegisterEmpty = 1 << 5,
        TransmitterEmptyStatus = 1 << 6,
        ImpendingError = 1 << 7
    };

    static SerialPort* try_to_create(uint16_t, uint16_t);
    void divisor_set(uint16_t);
    uint32_t baud_rate_get(void) const;

private:
    SerialPort(uint16_t, uint16_t);
    uint8_t read(Register) const;
    void write(Register, uint8_t) const;

    uint32_t m_divisor {1};
    uint16_t m_iobase;

    friend class SerialPortManager;
};

class SerialPortManager
{
MAKE_OBJECT_NON_COPYABLE(SerialPortManager)

public:
    static constexpr uint8_t s_max_amount_of_ports = 8;
    static constexpr uint16_t s_addresses[] = {
        0x3F8,
        0x2F8,
        0x3E8,
        0x2E8,
        0x5F8,
        0x4F8,
        0x5E8,
        0x4E8,
    };

    static SerialPortManager& instance_get(void);
    static bool is_initialized(void);
    static bool is_functional(void); // return true if at least one SerialPort is valid
    static bool initialize(uint16_t);
    static uint8_t receive(void);
    static void send(uint8_t val);

private:
    SerialPortManager() = default;
    static SerialPort* first_valid_port_get(void);

    static SerialPortManager s_instance;
    static SerialPort *s_ports[s_max_amount_of_ports];
    static bool s_initialized;
};

}