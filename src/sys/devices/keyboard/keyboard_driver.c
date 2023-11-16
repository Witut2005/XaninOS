
#pragma once

#include <lib/libc/hal.h>
#include <lib/libc/singal.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/input/input.h>

#define KEYBOARD_DRIVER_KEY_REMAP(from, to) \
    if (KeyInfo.character == from)          \
    KeyInfo.character = to

#pragma once

#include <lib/libc/hal.h>
#include <lib/screen/screen.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/terminal/backend/backend.h>

extern int screenshot(void);

#define KEYBOARD_ENCODER 0x60
#define ONBOARD_KEY_CONTROLLER 0x64
#define KEYBOARD_STATUS_REG 0x64
#define KEYBOARD_DATA_REG 0x60

void keyboard_driver_shift_remap_keys(void)
{
    if (KeyInfo.is_shift)
    {
        KEYBOARD_DRIVER_KEY_REMAP('-', '_');
        KEYBOARD_DRIVER_KEY_REMAP('1', '!');
        KEYBOARD_DRIVER_KEY_REMAP('2', '@');
        KEYBOARD_DRIVER_KEY_REMAP('3', '#');
        KEYBOARD_DRIVER_KEY_REMAP('4', '$');
        KEYBOARD_DRIVER_KEY_REMAP('5', '%');
        KEYBOARD_DRIVER_KEY_REMAP('6', '^');
        KEYBOARD_DRIVER_KEY_REMAP('7', '&');
        KEYBOARD_DRIVER_KEY_REMAP('8', '*');
        KEYBOARD_DRIVER_KEY_REMAP('9', '(');
        KEYBOARD_DRIVER_KEY_REMAP('0', ')');
        KEYBOARD_DRIVER_KEY_REMAP('=', '+');
        KEYBOARD_DRIVER_KEY_REMAP('[', '{');
        KEYBOARD_DRIVER_KEY_REMAP(']', '}');
        KEYBOARD_DRIVER_KEY_REMAP('/', '?');
        KEYBOARD_DRIVER_KEY_REMAP(';', ':');
        KEYBOARD_DRIVER_KEY_REMAP('`', '~');
        KEYBOARD_DRIVER_KEY_REMAP(',', '<');
        KEYBOARD_DRIVER_KEY_REMAP('.', '>');
        KEYBOARD_DRIVER_KEY_REMAP('/', '?');
        KEYBOARD_DRIVER_KEY_REMAP(0x5C, '|');
        KEYBOARD_DRIVER_KEY_REMAP(0x27, 0x22);
    }
}

void keyboard_driver(void)
{

    interrupt_disable();
    KeyInfo.scan_code = inbIO(KEYBOARD_DATA_REG);
    KeyInfo.character = keyboard_map[KeyInfo.scan_code];

    KeyInfo.is_pressed = true;

    switch (KeyInfo.scan_code)
    {
    case LSHIFT:
    {
        KeyInfo.is_shift = true;
        KeyInfo.character = '\0';
        break;
    }
    case LSHIFT_RELEASE:
    {
        KeyInfo.is_shift = false;
        break;
    }
    case RSHIFT:
    {
        KeyInfo.is_shift = true;
        KeyInfo.character = '\0';
        break;
    }
    case RSHIFT_RELEASE:
    {
        KeyInfo.is_shift = false;
        break;
    }
    case CAPS:
    {
        KeyInfo.is_caps = KeyInfo.is_caps ? false : true;
        break;
    }

    case PRINT_SCREEN_KEY:
    {
        int x_tmp = Screen.x, y_tmp = Screen.y;
        screenshot();
        eoi_send();
        KeyInfo.character = 0x0;
        Screen.x = x_tmp;
        Screen.y = y_tmp;

        break;
    }

    case L_ALT:
    {
        KeyInfo.is_alt = true;
        KeyInfo.character = 0x0;
        break;
    }
    case L_ALT_RELEASE:
    {
        KeyInfo.is_alt = false;
        break;
    }

    case L_CTRL:
    {
        KeyInfo.is_ctrl = true;
        KeyInfo.character = 0x0;
        break;
    }
    case L_CTRL_RELEASE:
    {
        KeyInfo.is_ctrl = false;
        break;
    }
    case F4_KEY:
    {
        KeyInfo.character = 0x0;
        break;
    }
    case F4_KEY_RELEASE:
    {
        KeyInfo.character = 0x0;
        break;
    }
    }

    if (KeyInfo.scan_code >= 128)
    {
        KeyInfo.character = 0x0;
        KeyInfo.is_pressed = false;
    }

    if (KeyInfo.is_caps)
    {
        if (KeyInfo.character >= 'a' && KeyInfo.character <= 'z')
        {
            KeyInfo.character -= 32;
        }
    }

    if (KeyInfo.is_shift)
    {
        if (KeyInfo.is_caps)
        {
            if (KeyInfo.character >= 'A' && KeyInfo.character <= 'Z')
            {
                KeyInfo.character += 32;
            }
        }

        else
        {
            if (KeyInfo.character >= 'a' && KeyInfo.character <= 'z')
            {
                KeyInfo.character -= 32;
            }
        }
    }

    if (KeyInfo.scan_code == PRINT_SCREEN_KEY_RELEASE)
        screenshot();

    keyboard_driver_shift_remap_keys();

    if (KeyInfo.scan_code == LSHIFT || KeyInfo.scan_code == CAPS)
        KeyInfo.character = '\0';

    if (KeyInfo.scan_code == BSPC)
        KeyInfo.character = '\0';

    if (keyboard_handle != NULL)
        keyboard_handle();

    __input_handle_observed_objects(&KeyInfo);

    // we need to set interrupts
    eoi_send();

    __input_call_handlers(KeyInfo);

    if (KeyInfo.is_ctrl && KeyInfo.character == 'c')
        exit();
}
