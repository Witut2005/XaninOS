
#pragma once

#include <sys/input/key_info.h>
#include <sys/input/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // NEW INPUT FUNCTIONS
    static inline bool is_break_code(uint8_t scan_code)
    {
        return scan_code >= 0x80;
    }

    static inline InputHandlerOptions __input_handler_options_create(uint8_t **args, bool type)
    {
        InputHandlerOptions options = {args, type};
        return options;
    }

    static inline InputHandler __input_handler_create(input_handler_t handler, InputHandlerOptions options)
    {
        InputHandler obj = {handler, options};
        return obj;
    }

    bool __sys_input_add_object_to_observe(const key_info_t *const KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options);
    bool __sys_input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove);
    void __sys_input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo);

    bool __sys_input_add_handler(InputHandler Handler);
    bool __sys_input_remove_handler(const input_handler_t Handler);
    bool __sys_input_remove_user_handlers(void);
    void __sys_input_call_handlers(key_info_t KeyboardDriverKeyInfo);

    char __sys_getchar(void);
    char __sys_getscan(void);
    xchar __sys_getxchar(void);
    xchar __sys_inputg(void);
    // key_info_t __sys_keyinfo_get(void);
    void __sys_keyinfo_get(key_info_t *);

#ifdef __cplusplus
}
#endif