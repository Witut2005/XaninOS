
#pragma once

#include <sys/devices/keyboard/scan_codes.h>
#include <sys/input/key_info.h>
#include <sys/input/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    // NEW INPUT FUNCTIONS
    static inline bool is_break_code(uint8_t scan_code)
    {
        return scan_code >= 0x80;
    }

    static inline bool input_is_nonprintable_scan_code(uint8_t scan_code)
    {
        return (scan_code == KBP_LEFT_ALT) | (scan_code == KBSP_RIGHT_ALT) | (scan_code == KBP_LEFT_SHIFT) | (scan_code == KBP_RIGHT_SHIFT) | (scan_code == KBP_LEFT_CONTROL) | (scan_code == KBSP_RIGHT_CONTROL);
    }

    static inline InputObservableOptions input_observed_object_options_create(bool ignore_break_codes)
    {
        InputObservableOptions obj = { ignore_break_codes };
        return obj;
    }

    static inline InputObservable input_observable_create(KeyInfo KeyboardInfo, InputObservableOptions options)
    {
        InputObservable obj = { KeyboardInfo, options };
        return obj;
    }

    static inline InputHandlerOptions input_handler_options_create(void* args, bool type)
    {
        InputHandlerOptions options = { args, type };
        return options;
    }

    static inline InputHandler input_handler_create(input_handler_t handler, InputHandlerOptions options)
    {
        InputHandler obj = { handler, options };
        return obj;
    }

    char __sys_getchar(void);
    char __sys_getscan(void);
    xchar __sys_getxchar(void);
    xchar __sys_inputg(void);
    // KeyInfo __sys_keyinfo_get(void);
    void __sys_keyinfo_get(KeyInfo*);
    bool __sys_is_normal_key_pressed(uint8_t scan_code);
    bool __sys_is_special_key_pressed(uint8_t scan_code);
    void __sys_input_character_mapper_set(char (*mapper)(uint8_t));
    void __sys_input_character_mapper_call(void);
    void __sys_input_prtsc_handler_set(input_scan_code_mapper_handler_t handler);

    bool __sys_input_add_object_to_observe(InputObservable Object);
    bool __sys_input_remove_object_from_observe(KeyInfo const* const KeyInfoToRemove);
    void __sys_input_handle_observed_objects(KeyInfo const* const KeyboardDriverKeyInfo);

    bool __sys_input_add_handler(InputHandler const* const Handler);
    bool __sys_input_remove_handler(const input_handler_t Handler);
    bool __sys_input_remove_user_handlers(void);
    void __sys_input_call_handlers(KeyInfo KeyboardDriverKeyInfo);

#ifdef __cplusplus
}
#endif
