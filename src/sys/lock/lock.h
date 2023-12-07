
#pragma once

#include <stdint.h>
#include <lib/libcpp/container/vector.hpp>

typedef uint32_t xanin_lock_id_t;

template <typename T>
xanin_lock_id_t lock(T *object);
bool unlock(uint32_t id);

template <typename T>
bool is_locked(T *id);
