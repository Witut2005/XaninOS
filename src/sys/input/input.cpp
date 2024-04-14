
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/key_map.h>

static key_info_t XaninGlobalKeyInfo;

static std::array<KeyboardModuleObservedObject, 100> KeyboardModuleObservedObjects;
static std::array<InputHandler, 100> InputModuleHandlers;

static void (*input_character_mapper)(uint8_t scan_code);
static InputScanCodeMapperHandlers XaninScanCodeMapperHandlers;

extern "C" int screenshot(void);

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

    key_info_t __input_global_key_info_get(void)
    {
        return XaninGlobalKeyInfo;
    }

    void __input_global_key_info_set(key_info_t KeyInfo)
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

        if (is_break_code(XaninGlobalKeyInfo.scan_code))
            XaninGlobalKeyInfo.character = 0x0;
    }

    void __input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code))
    {
        input_character_mapper = mapper;
    }

    void __input_scan_code_mapper_call(uint8_t scan_code)
    {
        return input_character_mapper(scan_code);
    }

    bool __input_is_normal_key_pressed(uint8_t scan_code)
    {
        return XaninGlobalKeyInfo.keys_pressed[scan_code];
    }

    bool __input_is_special_key_pressed(uint8_t scan_code)
    {
        return XaninGlobalKeyInfo.special_keys_pressed[scan_code];
    }

    InputHandler* input_module_handlers_get()
    {
        return InputModuleHandlers.begin().pointer();
    }

    void __input_handle_observed_objects(const key_info_t* const KeyboardDriverKeyInfo)
    {
        auto ObjectsToHandle = std::find(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [](const auto& a)
        { return a.valid(); });

        bool break_code = is_break_code(KeyboardDriverKeyInfo->scan_code);

        for (auto& it : ObjectsToHandle)
        {
            if (!((*it).Options.ignore_break_codes & break_code))
                memcpy((uint8_t*)it.pointer()->KeyInfo, (uint8_t*)KeyboardDriverKeyInfo, sizeof(key_info_t));
        }
    }

    void __input_init(void)
    {
        memset((uint8_t*)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
    }

    bool __input_add_object_to_observe(KeyboardModuleObservedObject Object)
    {
        auto ObjectInserted = find_first(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [](const auto& a)
        { return !a.valid(); });

        if (!ObjectInserted.valid())
            return false;

        *ObjectInserted = Object;

        return true;
    }

    bool __input_remove_object_from_observe(const key_info_t* const KeyInfoToRemove)
    {
        auto ObjectsToRemove = std::find(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), [=](auto a)
        { return a.pointer()->KeyInfo == KeyInfoToRemove; });

        if (!ObjectsToRemove.size())
            return false;

        for (auto& it : ObjectsToRemove)
            memset((uint8_t*)it.pointer()->KeyInfo, (uint8_t)NULL, sizeof(KeyboardModuleObservedObject));

        return true;
    }

    bool __input_add_handler(const InputHandler* const Handler)
    {
        auto HandlerToAdd = std::find_first(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto& a)
        { return a.pointer()->handler == NULL; });

        if (!HandlerToAdd.valid())
            return false;

        memcpy((uint8_t*)HandlerToAdd.pointer(), (uint8_t*)Handler, sizeof(InputHandler));
        return true;
    }

    void __input_call_handlers(key_info_t KeyboardDriverKeyInfo)
    {
        auto HandlersToCall = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto& a)
        { return a.pointer()->handler != NULL; });

        for (auto& a : HandlersToCall)
            a.pointer()->handler(KeyboardDriverKeyInfo, a.pointer()->options.args);
    }

    bool __input_remove_handler(const input_handler_t Handler)
    {
        auto HandlersToRemove = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [=](auto& a)
        { return a.pointer()->handler == Handler; });

        if (!HandlersToRemove.size())
            return false;

        for (auto& a : HandlersToRemove)
            memset((uint8_t*)a.pointer(), (uint8_t)NULL, sizeof(InputHandler));
        return true;
    }

    bool __input_remove_user_handlers(void)
    {
        auto HandlersToRemove = std::find(InputModuleHandlers.begin(), InputModuleHandlers.end(), [](auto& a)
        { return a.pointer()->options.type == USER_INPUT_HANDLER; });

        if (!HandlersToRemove.size())
            return false;

        for (auto& a : HandlersToRemove)
            memset((uint8_t*)a.pointer(), (uint8_t)NULL, sizeof(InputHandler));

        return true;
    }

    char __inputc(void)
    {
        return __inputg().character;
    }

    void __keyinfo_clear(void)
    {
        // memset((uint8_t*)&XaninGlobalKeyInfo, 0, sizeof(XaninGlobalKeyInfo));
    }

    key_info_t __keyinfo_get(void)
    {
        return XaninGlobalKeyInfo;
    }

    xchar __inputg(void)
    {

        key_info_t InputgKeyInfo;

        InputgKeyInfo.scan_code = XaninGlobalKeyInfo.scan_code = 0;
        // __keyinfo_clear();

        while ((InputgKeyInfo.scan_code == 0) || (InputgKeyInfo.scan_code >= 0x80))
            InputgKeyInfo = __keyinfo_get(); // break codes doesnt count

        xchar x;
        x.character = InputgKeyInfo.character;
        x.scan_code = InputgKeyInfo.scan_code;

        return x;
    }
}
