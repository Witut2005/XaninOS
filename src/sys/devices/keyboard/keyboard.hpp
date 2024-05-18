#pragma once
#include <stdint.h>
#include <lib/libcpp/class.hpp>
#include <sys/interrupts/idt/idt.h>
#include <sys/input/types.h>

namespace Device {

class Keyboard
{
public:
    MAKE_OBJECT_NON_COPYABLE(Keyboard);

    using leds_mask_t = uint8_t;

    enum ControllerPort : uint8_t
    {
        KeyboardEncoder = 0x60,
        OnboardKeyboardController = 0x64
    };

    enum Command : uint8_t
    {
        //Keyboard Encoder
        LEDset = 0xED,
        Echo = 0xEE,
        ScanCodeSet = 0xF0,
        KeyboardIdSend = 0xF2,
        ReapeatRateSet = 0xF3,
        Enable = 0xF4,
        Reset = 0xFF,

        //Onboard controller
        Read = 0x20,
        Write = 0x60,
        SelfTest = 0xAA,
        OnboardDisable = 0xAD,
        OnboardEnable = 0xAE,
        CPUSystemReset = 0xFE
    };

    enum SelfTestReturnStatus : uint8_t {
        Success = 0x55,
        Failure = 0xFC,
    };

    enum class Leds : uint8_t {
        Scroll,
        Num,
        Caps
    };

    ////////////////////////////////////////////////////////////////

    enum StatusRegisterMask : uint8_t {
        OutputBufferStatus = 1,
        InputBufferStatus = 2,
        System = 4,
        CommandData = 8,
        LockedFlag = 0x10,
        AuxOutputBufferFull = 0x20,
        Timeout = 0x40,
        ParityError = 0x80
    };


    enum BufferStatus : uint8_t {
        Empty,
        Full
    };

    enum SystemFlag : uint8_t {
        Default,
        BATSuccess,
    };

    enum CommandData : uint8_t {
        Data,
        Command
    };

    enum KeyboardLockStatus : uint8_t {
        Locked,
        NotLocked
    };

    enum Timeout : uint8_t {
        Ok,
        Timemout
    };

    enum ParityError : uint8_t {
        NoError,
        Error
    };

    ////////////////////////////////////////////////////////////////

    enum class KeyType {
        Normal,
        Special
    };

    ////////////////////////////////////////////////////////////////

    bool init(interrupt_vector_t vector);
    bool test(void);
    void reset(void);
    void cpu_reset(void);
    void leds_set(leds_mask_t);
    void handle(void);

    static Keyboard& the(void);

private:
    Keyboard() = default;

    void write(ControllerPort, uint8_t);
    uint8_t read(ControllerPort);
    void key_state_update(bool is_special_key, uint8_t scan_code);

    static Keyboard s_instance;

    KeyInfo m_key_info;
    bool m_initialized{ false };
    bool m_special_key_pressed{ false };
};

}