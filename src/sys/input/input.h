
#pragma once
#include <sys/input/key_info.h>
#include <sys/call/xanin_sys/calls/input/input.h>
#include "./types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool __input_is_ctrl_pressed(void);
    bool __input_is_shift_pressed(void);
    bool __input_is_alt_pressed(void);

    key_info_t __input_global_key_info_get(void);
    void __input_global_key_info_set(key_info_t KeyInfo);

    void __input_default_prtsc_handler(void);
    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler);
    void xanin_default_character_mapper(uint8_t scan_code);
    void __input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code));
    void __input_scan_code_mapper_call(uint8_t scan_code);
    bool __input_is_normal_key_pressed(uint8_t scan_code);
    bool __input_is_special_key_pressed(uint8_t scan_code);

    InputHandler *input_module_handlers_get();
    bool __input_add_object_to_observe(KeyboardModuleObservedObject Object);
    bool __input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove);
    void __input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo);

    bool __input_add_handler(const InputHandler *const Handler);
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