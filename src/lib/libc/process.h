
#pragma once

#define app_process_destroy() app_process_unregister()

void app_process_register(void (*deconstructor)(void), uint32_t number_of_pointers, ...);
void app_process_unregister(void);