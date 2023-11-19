
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/container/array/array.hpp>
#include <lib/libcpp/container/vector/vector.hpp>
#include <sys/input/input.h>
#include <lib/libcpp/algorithm.h>
#include <lib/libc/stdiox.h>

key_info_t KeyInfo = {0};

static std::array<KeyboardModuleObservedObject, 100> KeyboardModuleObservedObjects;
static std::array<InputHandler, 100> InputModuleHandlers;

static char (*input_character_mapper)(uint8_t scan_code);

extern "C"
{

    void __input_character_mapper_set(char (*mapper)(uint8_t scan_code))
    {
        input_character_mapper = mapper;
    }

    char __input_character_mapper_call(uint8_t scan_code)
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
