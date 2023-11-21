
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/key_map.h>

key_info_t KeyInfo = {0};

static std::array<KeyboardModuleObservedObject, 100> KeyboardModuleObservedObjects;
static std::array<InputHandler, 100> InputModuleHandlers;

static void (*input_character_mapper)(uint8_t scan_code);

extern "C"
{
#define KEYBOARD_DRIVER_KEY_REMAP(from, to) \
    if (KeyInfo.character == from)          \
    KeyInfo.character = to

    void xanin_default_character_mapper(uint8_t scan_code)
    {

        KeyInfo.character = keyboard_map[KeyInfo.scan_code];

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
            // int x_tmp = Screen.x, y_tmp = Screen.y;
            // screenshot();
            // keyboard_driver_clean_up();

            // KeyInfo.character = 0x0;
            // Screen.x = x_tmp;
            // Screen.y = y_tmp;

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

        // if (KeyInfo.scan_code == PRINT_SCREEN_KEY_RELEASE)
        //     screenshot();
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

        if (KeyInfo.scan_code == LSHIFT || KeyInfo.scan_code == CAPS)
            KeyInfo.character = '\0';

        if (KeyInfo.scan_code == BSPC)
            KeyInfo.character = '\0';
    }

    void __input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code))
    {
        input_character_mapper = mapper;
    }

    void __input_scan_code_mapper_call(uint8_t scan_code)
    {
        return input_character_mapper(scan_code);
    }

    InputHandler *input_module_handlers_get()
    {
        return InputModuleHandlers.begin().pointer();
    }

    void __input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo)
    {
        auto ObjectsToHandle = std::find(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [](const auto &a)
                                         { return a.valid(); });

        bool break_code = is_break_code(KeyboardDriverKeyInfo->scan_code);

        for (auto &it : ObjectsToHandle)
        {
            if (!((*it).Options.ignore_break_codes & break_code))
                memcpy((uint8_t *)it.pointer()->KeyInfo, (uint8_t *)KeyboardDriverKeyInfo, SIZE_OF(key_info_t));
        }
    }

    void __input_init(void)
    {
        memset((uint8_t *)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
    }

    bool __input_add_object_to_observe(KeyboardModuleObservedObject Object)
    {
        auto ObjectInserted = find_first(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [](const auto &a)
                                         { return !a.valid(); });

        if (!ObjectInserted.valid())
            return false;

        *ObjectInserted = Object;

        return true;
    }

    bool __input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove)
    {
        auto ObjectsToRemove = std::find(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [=](auto a)
                                         { return a.pointer()->KeyInfo == KeyInfoToRemove; });

        if (!ObjectsToRemove.size())
            return false;

        for (auto &it : ObjectsToRemove)
            memset((uint8_t *)it.pointer()->KeyInfo, (uint8_t)NULL, SIZE_OF(KeyboardModuleObservedObject));

        return true;
    }

    bool __input_add_handler(const InputHandler *const Handler)
    {
        auto HandlerToAdd = std::find_first(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto &a)
                                            { return a.pointer()->handler == NULL; });

        if (!HandlerToAdd.valid())
            return false;

        memcpy((uint8_t *)HandlerToAdd.pointer(), (uint8_t *)Handler, SIZE_OF(InputHandler));
        return true;
    }

    void __input_call_handlers(key_info_t KeyboardDriverKeyInfo)
    {
        auto HandlersToCall = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto &a)
                                        { return a.pointer()->handler != NULL; });

        for (auto &a : HandlersToCall)
            a.pointer()->handler(KeyboardDriverKeyInfo, a.pointer()->options.args);
    }

    bool __input_remove_handler(const input_handler_t Handler)
    {
        auto HandlersToRemove = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [=](auto &a)
                                          { return a.pointer()->handler == Handler; });

        if (!HandlersToRemove.size())
            return false;

        for (auto &a : HandlersToRemove)
            memset((uint8_t *)a.pointer(), (uint8_t)NULL, SIZE_OF(InputHandler));
        return true;
    }

    bool __input_remove_user_handlers(void)
    {
        auto HandlersToRemove = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto &a)
                                          { return a.pointer()->options.type == USER_INPUT_HANDLER; });

        if (!HandlersToRemove.size())
            return false;

        for (auto &a : HandlersToRemove)
            memset((uint8_t *)a.pointer(), (uint8_t)NULL, SIZE_OF(InputHandler));

        return true;
    }

    char __inputc(void)
    {
        return __inputg().character;
    }

    void __keyinfo_clear(void)
    {
        // memset((uint8_t*)&KeyInfo, 0, sizeof(KeyInfo));
    }

    key_info_t __keyinfo_get(void)
    {
        return KeyInfo;
    }

    xchar __inputg(void)
    {

        key_info_t InputgKeyInfo;

        InputgKeyInfo.scan_code = KeyInfo.scan_code = 0;
        // __keyinfo_clear();

        while ((InputgKeyInfo.scan_code == 0) || (InputgKeyInfo.scan_code >= 0x80))
            InputgKeyInfo = __keyinfo_get(); // break codes doesnt count

        xchar x;
        x.character = InputgKeyInfo.character;
        x.scan_code = InputgKeyInfo.scan_code;

        return x;
    }
}
