
#pragma once
#include <sys/input/key_info.h>

#ifdef __cplusplus
extern "C"{
#endif

int __input_module_add_object_to_observe(key_info_t* KeyInfoToObserve);
int __input_module_remove_object_from_observe(key_info_t* KeyInfoToRemove);
key_info_t __keyinfo_get(void);
char __inputc(void);
xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;