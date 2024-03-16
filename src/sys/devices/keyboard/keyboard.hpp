#pragma once
#include <stdint.h>
#include <lib/libcpp/class.hpp>
#include <sys/interrupts/idt/idt.h>

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

        //Onboard Keyboard
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

    enum ControllerStatusMask : uint8_t {};

    bool init(interrupt_vector_t vector);
    void reset(void);
    bool test(void);
    void handle(void);

    void leds_set(leds_mask_t);
    void cpu_reset(void);
    // void scan_code_set(uint8_t);

    static Keyboard& the(void);

private:
    Keyboard() = default;

    void write(ControllerPort, enum Command);
    uint8_t read(ControllerPort);

    static Keyboard s_instance;
    bool m_initialized{ false };
};

}