
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

constexpr auto TypeKernel = InputManager::EntryType::Kernel;
constexpr auto TypeUser = InputManager::EntryType::User;

InputManager InputManager::s_instance;

#define INPUT_DEFINE_CPP_WRAPPER1(return_type, method_name, params, param_name1) \
return_type input_##method_name params { \
return InputManager::the().method_name(param_name1); \
}

#define INPUT_DEFINE_CPP_WRAPPER2(return_type, method_name, params, param_name1, param_name2) \
return_type input_##method_name params { \
return InputManager::the().method_name(param_name1, param_name2); \
}

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
        KeyInfo& xanin_global_key_info = InputManager::the().key_info_ref_get();
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

    INPUT_DEFINE_CPP_WRAPPER1(void, mapper_set, (void(*mapper)(uint8_t scan_code)), mapper);
    INPUT_DEFINE_CPP_WRAPPER1(void, mapper_call, (uint8_t scan_code), scan_code);
    INPUT_DEFINE_CPP_WRAPPER1(void, handlers_call, (void), );
    INPUT_DEFINE_CPP_WRAPPER1(void, user_handlers_remove, (void), );
    INPUT_DEFINE_CPP_WRAPPER1(KeyInfo, key_info_get, (void), );

    bool input_is_normal_key_pressed(uint8_t scan_code)
    {
        return InputManager::the().is_key_pressed(scan_code, false);
    }

    bool input_is_special_key_pressed(uint8_t scan_code)
    {
        return InputManager::the().is_key_pressed(scan_code, true);
    }

    bool input_handler_add(InputHandler handler, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().add<InputManager::TableType::Handlers, InputManager::EntryType::Kernel>(handler) :
            InputManager::the().add<InputManager::TableType::Handlers, InputManager::EntryType::User>(handler);
    }


    bool input_handler_remove(int id, INPUT_TABLE_TYPE type)
    {
        return type == INPUT_KERNEL ? InputManager::the().remove<InputManager::TableType::Handlers, InputManager::EntryType::Kernel>(id) :
            InputManager::the().remove<InputManager::TableType::Handlers, InputManager::EntryType::User>(id);
    }

    xchar __inputg(void)
    {
        auto& input = InputManager::the();
        auto scan_code = input.key_info_get().scan_code;

        input.dirty_clear();
        while (!input.dirty_get());

        auto key_info = input.key_info_get();
        return { key_info.character, key_info.scan_code };
    }

    char __inputc(void)
    {
        return __inputg().character;
    }

} //extern "C"
