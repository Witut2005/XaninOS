#pragma once

#include <stdbool.h>
#include <sys/input/key_info.h>

typedef enum {
    INPUT_KERNEL,
    INPUT_USER
} INPUT_TABLE_TYPE;

typedef struct
{
    void* args;
    INPUT_TABLE_TYPE type;
} InputHandlerOptions;

typedef void (*input_handler_t)(KeyInfo, void*);

typedef struct
{
    input_handler_t handler;
    InputHandlerOptions options;
} InputHandler;

typedef void (*input_scan_code_mapper_handler_t)(void); // TODO find better type name

typedef struct
{
    input_scan_code_mapper_handler_t prtsc;
    // input_scan_code_mapper_handler_t shift;
    // input_scan_code_mapper_handler_t alt;
} InputSpecialKeyHandlers;
