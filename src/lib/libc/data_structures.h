#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <lib/libc/stdlibx.h>
#include <sys/macros.h>
struct stack_t
{
    uint32_t number_of_elements;
    uint32_t* end;
};

#ifndef __cplusplus
typedef struct stack_t stack_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void stack_create(stack_t** Stack);
void stack_push(stack_t* Stack, const address_t const value);
address_t stack_pop(stack_t* Stack);
bool stack_is_empty(stack_t* Stack);
void queue_create(void);
void binary_tree_create(void);
void tree_create(void);

#ifdef __cplusplus
}
#endif