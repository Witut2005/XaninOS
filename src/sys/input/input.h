
#pragma once
#include <sys/input/key_info.h>

struct KeyboardModuleObservedObjectOptions
{
    bool ignore_break_codes;
};
typedef struct KeyboardModuleObservedObjectOptions KeyboardModuleObservedObjectOptions;

struct KeyboardModuleObservedObject
{
    key_info_t *KeyInfo;
    KeyboardModuleObservedObjectOptions Options;
};
typedef struct KeyboardModuleObservedObject KeyboardModuleObservedObject;

struct InputModuleHandler
{
    bool (*handler)(key_info_t, uint8_t **);
    uint8_t **args;
};

typedef struct InputModuleHandler InputModuleHandler;

static inline bool is_break_code(uint8_t scan_code)
{
    return scan_code >= 0x80;
}

static inline InputModuleHandler input_module_handler_create(bool (*handler)(key_info_t, uint8_t **), uint8_t **args)
{
    InputModuleHandler obj = {handler, args};
    return obj;
}

#ifdef __cplusplus
extern "C"
{
#endif

    void __input_module_handle_observed_objects(key_info_t *KeyboardDriverKeyInfo);
    int __input_module_add_object_to_observe(key_info_t *KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options);
    int __input_module_remove_object_from_observe(key_info_t *KeyInfoToRemove);
    int __input_module_add_handler(InputModuleHandler Handler);
    void __input_module_call_handlers(key_info_t KeyboardDriverKeyInfo);
    key_info_t __keyinfo_get(void);
    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;