
#include <lib/libc/hal.h>
#include <sys/input/input.h>

key_info_t KeyInfo = {0};

static key_info_t* KeyboardModuleObservedObjects[100];

void __input_module_init(void)
{
    memset((uint8_t*)&KeyboardModuleObservedObjects, 0, sizeof(KeyboardModuleObservedObjects));
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