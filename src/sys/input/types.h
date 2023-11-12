#pragma once

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