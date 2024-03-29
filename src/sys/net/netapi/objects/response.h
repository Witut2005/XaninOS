
#pragma once

#include <stddef.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <sys/macros.h>

#define NETWORK_RESPONSE_WAIT(ms) msleep(ms)

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

void response_object_create(NetworkResponse** Object, uint32_t size_to_allocate);
void response_object_destroy(NetworkResponse** Object);

#ifdef __cplusplus
}
#endif