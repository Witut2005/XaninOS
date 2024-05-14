
#pragma once
#include <sys/input/key_info.h>
#include <sys/input/types.h>

// MAPPER WSKAZNIK DO KEY INFO CONST

#ifdef __cplusplus
extern "C"
{
#endif

    bool __input_is_ctrl_pressed(void);
    bool __input_is_shift_pressed(void);
    bool __input_is_alt_pressed(void);

    KeyInfo __input_global_key_info_get(void);
    void __input_global_key_info_set(KeyInfo KeyInfo);

    void __input_default_prtsc_handler(void);
    void __input_prtsc_handler_set(input_scan_code_mapper_handler_t handler);
    void xanin_default_character_mapper(uint8_t scan_code);
    void input_scan_code_mapper_call(uint8_t scan_code);
    void input_scan_code_mapper_set(void (*mapper)(uint8_t scan_code));
    bool input_is_normal_key_pressed(uint8_t scan_code);
    bool input_is_special_key_pressed(uint8_t scan_code);

    InputHandler* input_module_handlers_get();
    bool input_observable_add(InputObservable* Object, INPUT_TABLE_TYPE type);
    bool input_observable_remove(const KeyInfo* const KeyInfoToRemove);
    void input_obserables_update(const KeyInfo* const KeyboardDriverKeyInfo);

    bool input_handler_add(const InputHandler* const Handler, INPUT_TABLE_TYPE type);
    bool input_handler_remove(int id, INPUT_TABLE_TYPE type);
    void input_user_handlers_remove(void);
    void input_handlers_call(KeyInfo KeyboardDriverKeyInfo);

    KeyInfo __keyinfo_get(void);
    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern KeyInfo key_info;