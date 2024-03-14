#pragma once
#include <stdint.h>

namespace Device {

class Keyboard
{
public:
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

private:

};

}