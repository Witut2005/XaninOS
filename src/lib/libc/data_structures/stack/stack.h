#pragma once

#include <lib/libc/stdlibx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/macros.h>

typedef struct {
  uint32_t number_of_elements;
  uint32_t* end;
} stack_t;

#ifdef __cplusplus
extern "C" {
#endif

  void stack_create(stack_t** Stack);
  void stack_push(stack_t* Stack, const address_t value);
  address_t stack_pop(stack_t* Stack);
  bool stack_is_empty(stack_t* Stack);

#ifdef __cplusplus
}
#endif
