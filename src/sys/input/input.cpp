
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

static KeyInfo XaninGlobalKeyInfo;

static std::array<InputObservable, 100> InputObservables;
static std::array<InputHandler, 100> InputModuleHandlers;

static void (*input_character_mapper)(uint8_t scan_code);
static InputScanCodeMapperHandlers XaninScanCodeMapperHandlers;

extern "C" int screenshot(void);

constexpr auto Handlers = InputManager::TableType::Handlers;
constexpr auto Observables = InputManager::TableType::Observables;

constexpr auto TypeKernel = InputManager::EntryType::Kernel;
constexpr auto TypeUser = InputManager::EntryType::User;

extern "C"
{

    KeyInfo __input_global_key_info_get(void)
    {
        return XaninGlobalKeyInfo;
    }

    void __input_global_key_info_set(KeyInfo KeyInfo)
    {
        XaninGlobalKeyInfo = KeyInfo;
    }

    void __input_default_prtsc_handler(void)
    {
        int x_tmp = Screen.x, y_tmp = Screen.y;
        screenshot();

        XaninGlobalKeyInfo.character = 0x0;
        Screen.x = x_tmp;
        Screen.y = y_tmp;
    }

    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler)
    {
        XaninScanCodeMapperHandlers.prtsc = handler;
    }

#define INPUT_MODULE_KEY_REMAP_BEGIN(from, to)   \
    if (XaninGlobalKeyInfo.character == from) \
    XaninGlobalKeyInfo.character = to

#define INPUT_MODULE_KEY_REMAP(from, to)   \
    else if (XaninGlobalKeyInfo.character == from) \
    XaninGlobalKeyInfo.character = to

    void xanin_default_character_mapper(uint8_t scan_code)
    {
        XaninGlobalKeyInfo.character = keyboard_map[scan_code];

        if (is_break_code(scan_code)) {
            XaninGlobalKeyInfo.character = '\0';
            return;
        }

        // switch (XaninGlobalKeyInfo.scan_code)
        // {
        // case PRINT_SCREEN_KEY:
        // {
        //     XaninScanCodeMapperHandlers.prtsc();
        //     break;
        // }
        // }

        if (XaninGlobalKeyInfo.functional_keys.caps)
        {
            if (XaninGlobalKeyInfo.character >= 'a' && XaninGlobalKeyInfo.character <= 'z') {
                XaninGlobalKeyInfo.character -= 32;
            }
        }

        if (XaninGlobalKeyInfo.functional_keys.shift)
        {

            if (XaninGlobalKeyInfo.character >= 'a' && XaninGlobalKeyInfo.character <= 'z') {
                XaninGlobalKeyInfo.character -= 32;
            }

            //it will handle case when caps is true 
            else if (XaninGlobalKeyInfo.character >= 'A' && XaninGlobalKeyInfo.character <= 'Z') {
                XaninGlobalKeyInfo.character += 32;
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
        input_character_mapper = mapper;
    }

    void input_scan_code_mapper_call(uint8_t scan_code)
    {
        return input_character_mapper(scan_code);
    }

    bool input_is_normal_key_pressed(uint8_t scan_code)
    {
        return XaninGlobalKeyInfo.keys_pressed[scan_code];
    }

    bool input_is_special_key_pressed(uint8_t scan_code)
    {
        return XaninGlobalKeyInfo.special_keys_pressed[scan_code];
    }

    //changed and not tested yet
    void input_obserables_update(KeyInfo key_info)
    {
        InputManager::the().observables_update(key_info);
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

    void input_handlers_call(KeyInfo key_info)
    {
        return InputManager::the().handlers_call(key_info);
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

    KeyInfo __keyinfo_get(void)
    {
        return XaninGlobalKeyInfo;
    }

    xchar __inputg(void)
    {
        KeyInfo InputgKeyInfo;
        InputgKeyInfo.scan_code = XaninGlobalKeyInfo.scan_code = 0;

        while ((InputgKeyInfo.scan_code == 0) || (InputgKeyInfo.scan_code >= 0x80)) {
            InputgKeyInfo = __keyinfo_get(); // break codes doesnt count
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
}

void InputManager::observables_update(KeyInfo key_info)
{
    execute_on_tables<InputManager::TableType::Observables>([&key_info](InputObservable& observable) {
        if (observable.options.ignore_break_codes && is_break_code(key_info.scan_code)) {
            observable.key_info = key_info;
        }
    });
}

InputManager InputManager::s_instance;