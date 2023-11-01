
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>

key_info_t KeyInfo = {0};

std::array<KeyboardModuleObservedObject, 100> KeyboardModuleObservedObjects;
std::array<InputModuleHandler, 100> InputModuleHandlers;

extern "C"
{

    void __input_module_handle_observed_objects(key_info_t *KeyboardDriverKeyInfo)
    {
        for (int i = 0; i < KeyboardModuleObservedObjects.size(); i++)
        {
            if (KeyboardModuleObservedObjects[i].KeyInfo != NULL)
            {
                if (!(KeyboardModuleObservedObjects[i].Options.ignore_break_codes & is_break_code(KeyboardDriverKeyInfo->scan_code)))
                    memcpy((uint8_t *)KeyboardModuleObservedObjects[i].KeyInfo, (uint8_t *)KeyboardDriverKeyInfo, SIZE_OF(KeyboardModuleObservedObject));
            }
        }
    }

    void __input_module_init(void)
    {
        memset((uint8_t *)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
    }

    int __input_module_add_object_to_observe(key_info_t *KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options)
    {
        int index = -1;

        for (int i = 0; i < KeyboardModuleObservedObjects.size(); i++)
        {
            if (KeyboardModuleObservedObjects[i].KeyInfo == NULL)
                index = i;
        }

        if (index == -1)
            return -1;

        KeyboardModuleObservedObject obj = {KeyInfoToObserve, Options};
        KeyboardModuleObservedObjects[index] = obj;
        return 0;
    }

    int __input_module_remove_object_from_observe(key_info_t *KeyInfoToRemove)
    {
        int index = -1;

        for (int i = 0; i < KeyboardModuleObservedObjects.size(); i++)
        {
            if (KeyboardModuleObservedObjects[i].KeyInfo == KeyInfoToRemove)
                index = i;
        }

        if (index == -1)
            return -1;

        KeyboardModuleObservedObjects[index].KeyInfo = NULL;
        return 0;
    }

    int __input_module_add_handler(InputModuleHandler Handler)
    {

        int index = -1;

        for (int i = 0; i < InputModuleHandlers.size(); i++)
        {
            if (InputModuleHandlers[i].handler == NULL)
                index = i;
        }

        if (index == -1)
            return -1;

        InputModuleHandlers[index] = Handler;
        return 0;
    }

    void __input_module_call_handlers(key_info_t KeyboardDriverKeyInfo)
    {

        for (int i = 0; i < InputModuleHandlers.size(); i++)
        {
            if (InputModuleHandlers[i].handler != NULL)
                InputModuleHandlers[i].handler(KeyboardDriverKeyInfo, InputModuleHandlers[i].args);
        }
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
