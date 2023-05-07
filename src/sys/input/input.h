
#pragma once

#include <lib/libc/stdlibx.h>
#include <sys/input/key_info.h>

#ifdef __cplusplus
extern "C"{
#endif

char inputc(void);
xchar inputg(void);


#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;