
#pragma once

#include <stdint.h>

namespace Device
{
class SerialPort
{

public:
    static SerialPort* try_to_create(uint16_t);
    uint32_t baud_rate_get(void) const;

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

private:
    SerialPort(uint16_t);
    uint8_t read(Register);
    void write(Register, uint8_t);

    void dlab_toggle(void);
    void dlab_set(bool);
    void dlab_get(void);

    uint32_t divisor {1};
    uint16_t io_base;

    friend class SerialPortManager;
};

class SerialPortManager
{
public:
    static bool is_initialized(void);
    static bool initialize(void);
    static uint8_t receive(void);
    static bool send(uint8_t val);

private:
    static SerialPort *ports[SerialPort::s_max_amount_of_ports];
};

}