#pragma once

#include <stdbool.h>

typedef struct
{
    bool ignore_break_codes;
}InputObservableOptions;

typedef struct
{
    key_info_t* KeyInfo;
    InputObservableOptions Options;
}InputObservable;

enum INPUT_HANDLER_TYPES
{
    KERNEL_INPUT_HANDLER,
    USER_INPUT_HANDLER
};

typedef struct
{
    uint8_t** args;
    bool type;
}InputHandlerOptions;

typedef void (*input_handler_t)(key_info_t, uint8_t**);

typedef struct
{
    input_handler_t handler;
    InputHandlerOptions options;
}InputHandler;

typedef void (*input_scan_code_mapper_handler_t)(void); //TODO find better type name

typedef struct
{
    input_scan_code_mapper_handler_t prtsc;
    // input_scan_code_mapper_handler_t shift;
    // input_scan_code_mapper_handler_t alt;
}InputScanCodeMapperHandlers;
