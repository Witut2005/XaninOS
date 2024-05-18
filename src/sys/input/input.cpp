
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>
#include <sys/input/input.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/key_map.h>

static void (*input_character_mapper)(uint8_t scan_code);
static InputSpecialKeyHandlers xanin_special_key_handlers;

#define INPUT_MODULE_KEY_REMAP_BEGIN(from, to)   \
if (xanin_global_key_info.character == from) \
xanin_global_key_info.character = to

#define INPUT_MODULE_KEY_REMAP(from, to)   \
else if (xanin_global_key_info.character == from) \
xanin_global_key_info.character = to

extern "C" int screenshot(void);

constexpr auto Handlers = InputManager::TableType::Handlers;
constexpr auto Observables = InputManager::TableType::Observables;

constexpr auto TypeKernel = InputManager::EntryType::Kernel;
constexpr auto TypeUser = InputManager::EntryType::User;

void InputManager::observables_update(void)
{
    execute_on_tables<InputManager::TableType::Observables>([this](InputObservable& observable) {
        if (observable.options.ignore_break_codes && is_break_code(m_key_info.scan_code)) {
            observable.key_info = m_key_info;
        }
    });
}

InputManager InputManager::s_instance;

extern "C"
{



    void __input_default_prtsc_handler(void)
    {
        int x_tmp = Screen.x, y_tmp = Screen.y;
        screenshot();

        Screen.x = x_tmp;
        Screen.y = y_tmp;
    }

    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler)
    {
        xanin_special_key_handlers.prtsc = handler;
    }


    void xanin_default_character_mapper(uint8_t scan_code)
    {
        KeyInfo& xanin_global_key_info = InputManager::s_instance.m_key_info;
        xanin_global_key_info.character = keyboard_map[scan_code];

        if (is_break_code(scan_code)) {
            xanin_global_key_info.character = '\0';
            return;
        }

        // switch (xanin_global_key_info.scan_code)
        // {
        // case PRINT_SCREEN_KEY:
        // {
        //     XaninScanCodeMapperHandlers.prtsc();
        //     break;
        // }
        // }

        if (xanin_global_key_info.functional_keys.caps)
        {
            if (xanin_global_key_info.character >= 'a' && xanin_global_key_info.character <= 'z') {
                xanin_global_key_info.character -= 32;
            }
        }

        if (xanin_global_key_info.functional_keys.shift)
        {

            if (xanin_global_key_info.character >= 'a' && xanin_global_key_info.character <= 'z') {
                xanin_global_key_info.character -= 32;
            }

            //it will handle case when caps is true 
            else if (xanin_global_key_info.character >= 'A' && xanin_global_key_info.character <= 'Z') {
                xanin_global_key_info.character += 32;
            }

            INPUT_MODULE_KEY_REMAP_BEGIN('-', '_');
            INPUT_MODULE_KEY_REMAP('1', '!');
            INPUT_MODULE_KEY_REMAP('2', '@');
            INPUT_MODULE_KEY_REMAP('3', '#');
            INPUT_MODULE_KEY_REMAP('4', '$');
            INPUT_MODULE_KEY_REMAP('5', '%');
            INPUT_MODULE_KEY_REMAP('6', '^');
            INPUT_MODULE_KEY_REMAP('7', '&');
            INPUT_MODULE_KEY_REMAP('8', '*');
            INPUT_MODULE_KEY_REMAP('9', '(');
            INPUT_MODULE_KEY_REMAP('0', ')');
            INPUT_MODULE_KEY_REMAP('=', '+');
            INPUT_MODULE_KEY_REMAP('[', '{');
            INPUT_MODULE_KEY_REMAP(']', '}');
            INPUT_MODULE_KEY_REMAP('/', '?');
            INPUT_MODULE_KEY_REMAP(';', ':');
            INPUT_MODULE_KEY_REMAP('`', '~');
            INPUT_MODULE_KEY_REMAP(',', '<');
            INPUT_MODULE_KEY_REMAP('.', '>');
            INPUT_MODULE_KEY_REMAP('/', '?');
            INPUT_MODULE_KEY_REMAP(0x5C, '|');
            INPUT_MODULE_KEY_REMAP(0x27, 0x22);
        }

    }

    void input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code))
    {
        InputManager::the().mapper_set(mapper);
    }

    void input_scan_code_mapper_call(uint8_t scan_code)
    {
        InputManager::the().mapper_call(scan_code);
    }

    bool input_is_normal_key_pressed(uint8_t scan_code)
    {
        return InputManager::the().is_key_pressed(scan_code, false);
    }

    bool input_is_special_key_pressed(uint8_t scan_code)
    {
        return InputManager::the().is_key_pressed(scan_code, true);
    }

    //changed and not tested yet
    void input_observables_update(void)
    {
        InputManager::the().observables_update();
    }

    bool input_observable_add(InputObservable* observable, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().add<InputManager::TableType::Observables, InputManager::EntryType::Kernel>(*observable) :
            InputManager::the().add<InputManager::TableType::Observables, InputManager::EntryType::User>(*observable);
    }

    bool input_observable_remove(int id, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().remove<InputManager::TableType::Observables, InputManager::EntryType::Kernel>(id) :
            InputManager::the().remove<InputManager::TableType::Observables, InputManager::EntryType::User>(id);
    }

    bool input_handler_add(InputHandler handler, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().add<InputManager::TableType::Handlers, InputManager::EntryType::Kernel>(handler) :
            InputManager::the().add<InputManager::TableType::Handlers, InputManager::EntryType::User>(handler);
    }

    void input_handlers_call(void)
    {
        InputManager::the().handlers_call();
    }

    bool input_handler_remove(int id, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().remove<InputManager::TableType::Handlers, InputManager::EntryType::Kernel>(id) :
            InputManager::the().remove<InputManager::TableType::Handlers, InputManager::EntryType::User>(id);
    }

    void input_user_handlers_remove(void)
    {
        InputManager::the().user_handlers_remove();
    }

    KeyInfo __key_info_get(void)
    {
        return InputManager::the().key_info_get();
    }

    // xchar __inputg(void)
    // {
    //     // KeyInfo InputgKeyInfo;
    //     // InputgKeyInfo.scan_code = xanin_global_key_info.scan_code = 0;
    //     // InputManager::the().add<InputManager::TableType::Observables, InputManager::EntryType::Kernel>()
    //     auto key_info = InputManager::the().key_info_get();
    //     KeyInfo current_key_info;
    //     current_key_info.scan_code = key_info.scan_code = 0;

    //     do {
    //         current_key_info = InputManager::the().key_info_get();
    //     } while (current_key_info.scan_code == key_info.scan_code || is_break_code(current_key_info.scan_code));

    //     return { current_key_info.character, current_key_info.scan_code };
    // }

    xchar __inputg(void)
    {
        KeyInfo InputgKeyInfo;
        InputgKeyInfo.scan_code = InputManager::the().m_key_info.scan_code = 0;

        while ((InputgKeyInfo.scan_code == 0) || (InputgKeyInfo.scan_code >= 0x80)) {
            InputgKeyInfo = InputManager::the().m_key_info; // break codes doesnt count
        }

        xchar x;
        x.character = InputgKeyInfo.character;
        x.scan_code = InputgKeyInfo.scan_code;

        return x;
    }


    char __inputc(void)
    {
        return __inputg().character;
    }

} //extern "C"
