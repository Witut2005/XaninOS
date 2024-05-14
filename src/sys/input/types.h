#pragma once

#include <stdbool.h>
#include <sys/input/key_info.h>

typedef struct
{
    bool ignore_break_codes;
}InputObservableOptions;

typedef struct
{
    KeyInfo* key_info;
    InputObservableOptions options;
}InputObservable;

typedef enum
{
    INPUT_KERNEL,
    INPUT_USER
}INPUT_TABLE_TYPE;

typedef struct
{
    uint8_t** args;
    bool type;
}InputHandlerOptions;

typedef void (*input_handler_t)(KeyInfo, uint8_t**);

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
