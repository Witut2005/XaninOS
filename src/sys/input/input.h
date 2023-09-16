
#pragma once
#include <sys/input/key_info.h>

#ifdef __cplusplus
extern "C"{
#endif

key_info_t __keyinfo_get(void);
char __inputc(void);
xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;