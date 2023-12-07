
#pragma once

#include <lib/libc/stdlibx.h>

template <typename T>
bool __storage_add(const char *name, T *value);
template <typename T>
void __storage_set(const char *name, T *value);
address_t __storage_get(const char *name);
void __storage_remove(const char *name);