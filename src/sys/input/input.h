
#pragma once
#include <sys/input/key_info.h>
#include <sys/call/xanin_sys/calls/input/input.h>
#include "./types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool __input_add_object_to_observe(const key_info_t *const KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options);
    bool __input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove);
    void __input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo);

    bool __input_add_handler(InputHandler Handler);
    bool __input_remove_handler(const input_handler_t Handler);
    bool __input_remove_user_handlers(void);
    void __input_call_handlers(key_info_t KeyboardDriverKeyInfo);

    key_info_t __keyinfo_get(void);
    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;