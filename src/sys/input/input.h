
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

enum INPUT_HANDLER_TYPES
{
    KERNEL_INPUT_HANDLER,
    USER_INPUT_HANDLER
};

struct InputHandlerOptions
{
    uint8_t **args;
    bool type;
};
typedef struct InputHandlerOptions InputHandlerOptions;
typedef bool (*input_handler_t)(key_info_t, uint8_t **);

struct InputHandler
{
    input_handler_t handler;
    InputHandlerOptions options;
};
typedef struct InputHandler InputHandler;

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

#ifdef __cplusplus
extern "C"
{
#endif

    void __input_handle_observed_objects(const key_info_t *const KeyboardDriverKeyInfo);
    bool __input_add_object_to_observe(const key_info_t *const KeyInfoToObserve, KeyboardModuleObservedObjectOptions Options);
    int __input_remove_object_from_observe(const key_info_t *const KeyInfoToRemove);
    int __input_add_handler(InputHandler Handler);
    void __input_call_handlers(key_info_t KeyboardDriverKeyInfo);
    void __input_remove_handler(const input_handler_t Handler);
    void __input_remove_user_handlers(void);
    key_info_t __keyinfo_get(void);
    char __inputc(void);
    xchar __inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;