
#include <stdint.h>
#include <stdarg.h>
#include <lib/libc/stdlibx.h>

// static int process_id = 0;
static uint32_t number_of_pointers_to_clear = 0;
static uint8_t** process_pointers = NULL;
static void (*process_deconstructor)(void);

void app_process_register(void (*deconstructor)(void), uint32_t number_of_pointers, ...)
{
    va_list args;
    va_start(args, number_of_pointers);

    process_pointers = (uint8_t**)calloc(number_of_pointers * sizeof(uint8_t*));

    for(int i = 0; i < number_of_pointers; i++)
        process_pointers[i] = va_arg(args, uint8_t*);

    process_deconstructor = deconstructor;

    // process_id = true;
}

void app_process_unregister(void)
{
    for(int i = 0; number_of_pointers_to_clear; i++)
        free(process_pointers[i]);
    process_deconstructor();
}
