#pragma once
#include <stdint.h>
#include <lib/libcpp/class.hpp>

namespace Device {

class Keyboard
{
public:
    MAKE_OBJECT_NON_COPYABLE(Keyboard);

    enum class ControllerPorts : uint8_t
    {
        KeyboardEncoder = 0x60,
        OnboardKeyboardController = 0x64
    };

    enum class Registers : uint8_t
    {
        InputBuffer = 0x60,
        Command = 0x60,
        Status = 0x64,
        OnboardCommand = 0x64
    };

    enum ControllerStatusMask : uint8_t {};

    bool init(interrupt_vector_t vector);
    void reset(void);
    bool test(void);
    void handle(void);

    static Keyboard& the(void);

private:
    Keyboard() = default;

    void write(Registers) const;
    uint8_t read(Registers) const;

    static Keyboard s_instance;
    bool m_initialized{ false };
};

}