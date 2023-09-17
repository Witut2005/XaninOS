
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/array.h>
#include <lib/libcpp/vector.hpp>
#include <sys/input/input.h>

key_info_t KeyInfo = {0};

static std::array<key_info_t*, 100> KeyboardModuleObservedObjects;

extern "C" 
{

void __input_module_init(void)
{
    memset((uint8_t*)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
}

int __input_module_add_object_to_observe(key_info_t* KeyInfoToObserve)
{
    int index = KeyboardModuleObservedObjects.find_other_than(NULL);

    if(index == -1)
        return -1;

    KeyboardModuleObservedObjects[index] = KeyInfoToObserve;
    return 0;
}

int __input_module_remove_object_from_observe(key_info_t* KeyInfoToRemove)
{
    int index = KeyboardModuleObservedObjects.find(KeyInfoToRemove);

    if(index == -1)
        return -1;

    KeyboardModuleObservedObjects[index] = NULL;
    return 0;
}

char __inputc(void)
{
    return __inputg().character;
}

void __keyinfo_clear(void)
{
    memset((uint8_t*)&KeyInfo, 0, sizeof(KeyInfo));
}

key_info_t __keyinfo_get(void)
{
    return KeyInfo;
}

xchar __inputg(void)
{

    key_info_t InputgKeyInfo;

    InputgKeyInfo.scan_code = 0;
    __keyinfo_clear();

    while(!InputgKeyInfo.scan_code) InputgKeyInfo = __keyinfo_get(); // break codes doesnt count
    
    xchar x;
    x.character = InputgKeyInfo.character;
    x.scan_code = InputgKeyInfo.scan_code;

    return x;
}

}