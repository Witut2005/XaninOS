
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

constexpr auto Handlers = InputManager::TableTypes::Handlers;
constexpr auto Observables = InputManager::TableTypes::Observables;

constexpr auto TypeKernel = InputManager::EntryType::Kernel;
constexpr auto TypeUser = InputManager::EntryType::User;

extern "C"
{
    bool __input_is_ctrl_pressed(void)
    {
        return XaninGlobalKeyInfo.keys_pressed[KBP_LEFT_CONTROL] | XaninGlobalKeyInfo.special_keys_pressed[KBSP_RIGHT_CONTROL];
    }

    bool __input_is_shift_pressed(void)
    {
        return XaninGlobalKeyInfo.keys_pressed[KBP_LEFT_SHIFT] | XaninGlobalKeyInfo.keys_pressed[KBP_RIGHT_SHIFT];
    }

    bool __input_is_alt_pressed(void)
    {
        return XaninGlobalKeyInfo.keys_pressed[KBP_LEFT_ALT] | XaninGlobalKeyInfo.keys_pressed[KBSP_RIGHT_ALT];
    }

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

#define KEYBOARD_DRIVER_KEY_REMAP(from, to)   \
    if (XaninGlobalKeyInfo.character == from) \
    XaninGlobalKeyInfo.character = to

    void xanin_default_character_mapper(uint8_t scan_code)
    {

        XaninGlobalKeyInfo.character = keyboard_map[scan_code];

        switch (XaninGlobalKeyInfo.scan_code)
        {
        case PRINT_SCREEN_KEY:
        {
            XaninScanCodeMapperHandlers.prtsc();
            break;
        }
        }

        if (XaninGlobalKeyInfo.is_caps)
        {
            if (XaninGlobalKeyInfo.character >= 'a' && XaninGlobalKeyInfo.character <= 'z')
                XaninGlobalKeyInfo.character -= 32;
        }

        if (__input_is_shift_pressed())
        {
            if (XaninGlobalKeyInfo.is_caps)
            {
                if (XaninGlobalKeyInfo.character >= 'A' && XaninGlobalKeyInfo.character <= 'Z')
                {
                    XaninGlobalKeyInfo.character += 32;
                }
            }

            else
            {
                if (XaninGlobalKeyInfo.character >= 'a' && XaninGlobalKeyInfo.character <= 'z')
                {
                    XaninGlobalKeyInfo.character -= 32;
                }
            }
        }

        if (__input_is_shift_pressed())
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

        if (is_break_code(XaninGlobalKeyInfo.scan_code)) {
            XaninGlobalKeyInfo.character = 0x0;
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

    InputHandler* input_module_handlers_get()
    {
        return InputModuleHandlers.begin().pointer();
    }

    //changed and not tested yet
    void input_obserables_update(const KeyInfo* KeyboardDriverKeyInfo)
    {
        bool break_code = is_break_code(KeyboardDriverKeyInfo->scan_code);

        for (auto& a : InputObservables)
        {
            if (!(a.options.ignore_break_codes & break_code)) {
                a.key_info = *KeyboardDriverKeyInfo;
            }
        }
    }

    bool input_observable_add(InputObservable* observable, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().add<InputManager::TableTypes::Observables, InputManager::EntryType::Kernel>(*observable) :
            InputManager::the().add<InputManager::TableTypes::Observables, InputManager::EntryType::User>(*observable);
    }

    bool input_observable_remove(const KeyInfo* const KeyInfoToRemove)
    {
        return true;
    }

    bool input_handler_add(const InputHandler* Handler, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().add<InputManager::TableTypes::Handlers, InputManager::EntryType::Kernel>(*Handler) :
            InputManager::the().add<InputManager::TableTypes::Handlers, InputManager::EntryType::User>(*Handler);
    }

    void input_handlers_call(KeyInfo key_info)
    {
        return InputManager::the().handlers_call(key_info);
    }

    bool input_handler_remove(int id, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().remove<InputManager::TableTypes::Handlers, InputManager::EntryType::Kernel>(id) :
            InputManager::the().remove<InputManager::TableTypes::Handlers, InputManager::EntryType::User>(id);
    }

    void input_user_handlers_remove(void)
    {
        InputManager::the().user_handlers_remove();
    }

    void __keyinfo_clear(void)
    {
        // memset((uint8_t*)&XaninGlobalKeyInfo, 0, sizeof(XaninGlobalKeyInfo));
    }

    KeyInfo __keyinfo_get(void)
    {
        return XaninGlobalKeyInfo;
    }

    xchar __inputg(void)
    {
        KeyInfo InputgKeyInfo;
        InputgKeyInfo.scan_code = XaninGlobalKeyInfo.scan_code = 0;

        while ((InputgKeyInfo.scan_code == 0) || (InputgKeyInfo.scan_code >= 0x80))
            InputgKeyInfo = __keyinfo_get(); // break codes doesnt count

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

void InputManager::scan_code_mapper_set(void) {}

void InputManager::handlers_call(KeyInfo key_info)
{
    execute_on_tables<InputManager::TableTypes::Handlers>([&key_info](const InputHandler& handler) {handler.handler(key_info, handler.options.args);});
}

void InputManager::observables_update(KeyInfo key_info)
{
    execute_on_tables<InputManager::TableTypes::Observables>([&key_info](InputObservable& observable) {
        if (observable.options.ignore_break_codes && is_break_code(key_info.scan_code)) {
            observable.key_info = key_info;
        }
    });
}

void InputManager::user_handlers_remove(void)
{
    m_handlers.user.clear();
}

InputManager InputManager::s_instance;