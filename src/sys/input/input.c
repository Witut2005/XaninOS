
#include <lib/libc/hal.h>
#include <sys/input/input.h>

key_info_t KeyInfo = {0};

char inputc(void)
{
    return inputg().character;
}

void __keyinfo_clear(void)
{
    memset((uint8_t*)&KeyInfo, 0, sizeof(KeyInfo));
}

key_info_t __keyinfo_get(void)
{
    return KeyInfo;
}

xchar inputg(void)
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