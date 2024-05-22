
#pragma once
#include <sys/input/key_info.h>
#include <sys/input/types.h>

// MAPPER WSKAZNIK DO KEY INFO CONST
#define INPUT_DECLARE_CPP_WRAPPER(return_type, method, params) \
return_type input_##method params;

#ifdef __cplusplus
extern "C"
{
#endif

    KeyInfo input_key_info_get(void);

    bool input_is_normal_key_pressed(uint8_t scan_code);
    bool input_is_special_key_pressed(uint8_t scan_code);

    void __input_default_prtsc_handler(void);
    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler);

    void xanin_default_character_mapper(uint8_t scan_code);

    INPUT_DECLARE_CPP_WRAPPER(void, mapper_call, (uint8_t scan_code));
    INPUT_DECLARE_CPP_WRAPPER(void, mapper_set, (void(*mapper)(uint8_t scan_code)));
    INPUT_DECLARE_CPP_WRAPPER(bool, handler_add, (InputHandler handler));
    INPUT_DECLARE_CPP_WRAPPER(bool, handler_remove, (int id, INPUT_TABLE_TYPE type));
    INPUT_DECLARE_CPP_WRAPPER(void, user_handlers_remove, (void));
    INPUT_DECLARE_CPP_WRAPPER(void, handlers_call, (void));
    INPUT_DECLARE_CPP_WRAPPER(bool, is_break_code, (uint8_t));

    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern KeyInfo key_info;