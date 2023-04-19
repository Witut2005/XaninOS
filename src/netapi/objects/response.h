
#pragma once

#include <stddef.h>
#include <libc/stdlibx.h>

struct NetworkResponse
{
    bool success;
    address_t data;
};


#ifndef __cplusplus
typedef struct NetworkResponse NetworkResponse;
#endif

#ifdef __cplusplus
extern "C"{
#endif

NetworkResponse* response_object_create(NetworkResponse** Object, uint32_t size_to_allocate);

#ifdef __cplusplus
}
#endif