
#pragma once

#include <stdint.h>

namespace Device
{
    class SerialPort
    {

    public:
        static bool probe(uint16_t);
        static SerialPort &create(uint16_t);
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

        enum InterruptMode : uint8_t
        {
            DataAvailable = 1,
            TransmitterEmpty = 1 << 1,
            BreakOrError = 1 << 2,
            StatusChange = 1 << 3,
        };

    private:
        SerialPort(uint16_t);
        void write(Register, uint8_t);
        uint8_t read(Register);

        void dlab_toggle(void);
        void dlab_set(bool);
        void dlab_get(void);

        uint32_t m_divisor;
    };

    class SerialPortManager
    {
    public:
        static void initialize(void);

    private:
        static SerialPort *m_ports[SerialPort::s_max_amount_of_ports];
    };

}