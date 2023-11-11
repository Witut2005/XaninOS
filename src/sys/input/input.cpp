
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/stdiox.h>

key_info_t KeyInfo = {0};

std::array<KeyboardModuleObservedObject, 100> KeyboardModuleObservedObjects;
std::array<InputHandler, 100> InputModuleHandlers;

extern "C"
{

    void __input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo)
    {
        auto ObjectsToHandle = std::find(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(), 
                                            [](const auto& a){return a.valid();});

        for(auto& it : ObjectsToHandle)
            if (!((*it).Options.ignore_break_codes & is_break_code(KeyboardDriverKeyInfo->scan_code)))
                memcpy((uint8_t *)it.pointer()->KeyInfo, (uint8_t *)KeyboardDriverKeyInfo, SIZE_OF(KeyboardModuleObservedObject));
    }

    void __input_init(void)
    {
        memset((uint8_t *)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
    }

    bool __input_add_object_to_observe(const key_info_t *const KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options)
    {
        auto ObjectInserted = find_first(KeyboardModuleObservedObjects.begin(), KeyboardModuleObservedObjects.end(),
                                            [](const auto& a){return !a.valid();});

        if(!ObjectInserted.valid())
            return false;

        KeyboardModuleObservedObject obj = {(key_info_t *)(KeyInfoToObserve), Options};
        *ObjectInserted = obj;

        return true;
    }

    int __input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove)
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

    int __input_add_handler(InputHandler Handler)
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

    void __input_call_handlers(key_info_t KeyboardDriverKeyInfo)
    {
        for (int i = 0; i < InputModuleHandlers.size(); i++)
        {
            if (InputModuleHandlers[i].handler != NULL)
                InputModuleHandlers[i].handler(KeyboardDriverKeyInfo, InputModuleHandlers[i].options.args);
        }
    }

    void __input_remove_handler(const input_handler_t Handler)
    {
        for (int i = 0; i < InputModuleHandlers.size(); i++)
        {
            if (InputModuleHandlers[i].handler == Handler)
                InputModuleHandlers[i].handler = NULL;
        }
    }

    void __input_remove_user_handlers(void)
    {
        for (int i = 0; i < InputModuleHandlers.size(); i++)
        {
            if (InputModuleHandlers[i].options.type == USER_INPUT_HANDLER)
                InputModuleHandlers[i].handler = NULL;
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
