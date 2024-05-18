
#pragma once
#include <sys/input/key_info.h>
#include <sys/input/types.h>

// MAPPER WSKAZNIK DO KEY INFO CONST

#ifdef __cplusplus
extern "C"
{
#endif

    KeyInfo __key_info_get(void);

    bool input_is_normal_key_pressed(uint8_t scan_code);
    bool input_is_special_key_pressed(uint8_t scan_code);

    void __input_default_prtsc_handler(void);
    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler);

    void xanin_default_character_mapper(uint8_t scan_code);
    void input_scan_code_mapper_call(uint8_t scan_code);
    void input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code));

    bool input_observable_add(InputObservable* observable, INPUT_TABLE_TYPE type);
    bool input_observable_remove(int id, INPUT_TABLE_TYPE type);

    bool input_handler_add(InputHandler handler, INPUT_TABLE_TYPE type);
    bool input_handler_remove(int id, INPUT_TABLE_TYPE type);

    void input_user_handlers_remove(void);
    void input_user_observables_remove(void);

    void input_observables_update(void);
    void input_handlers_call(void);

    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern KeyInfo key_info;