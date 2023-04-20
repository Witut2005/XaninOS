
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define XANIN_PMMNGR_BLOCK_SIZE 4096

extern void(*keyboard_handle)(void);

#define KEYBOARD_KEYSTROKE_HANLDER_LOAD(handler) keyboard_handle = handler
#define KEYBOARD_KEYSTROKE_HANLDER_UNLOAD() keyboard_handle = NULL 

typedef uint8_t* address_t;
typedef int __STATUS;

// #ifndef NULL
// #define NULL 0x0
// #endif

struct CmosTime
{
    uint8_t seconds; 
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekday;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t century;
    

}__attribute__((packed));

typedef struct CmosTime CmosTime;
extern CmosTime SystemTime;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

extern char* daysLUT[7];

extern char command_buffer[1000];
extern char* keyboard_command;

extern uint8_t* null_memory_region;
struct 
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
}typedef reg_t;

struct
{
    uint16_t cs;
    uint16_t es;
    uint16_t ds;
    uint16_t ss;
    uint16_t fs;
    uint16_t gs;
}typedef seg_t;

extern reg_t Register;
extern seg_t SegmentRegister;

struct xchar
{
    char character;
    uint8_t scan_code;
};

#ifndef __cplusplus
typedef struct xchar xchar;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern bool key_pressed(void);
extern void wait_key(uint8_t key);
extern char getchar(void);
extern char getscan(void);
extern void keyboard_buffer_refresh(uint16_t* screen_buffer);
extern CmosTime* time_get(CmosTime* Time);
extern uint8_t floppy_type_get_cmos(void);
extern void get_cpu_speed(void);
extern uint32_t randomizer;
extern void swap_int(int *xp, int *yp);
extern void swap_char(char* xp, char* yp);
extern void swap_short(uint16_t* xp, uint16_t* yp);
extern void srand(uint32_t seed);
extern uint32_t memory_map_get_cmos(void);
extern void int_swap(int *xp, int *yp);
extern void bubble_sort(int* array, int n);  extern void merge(int* array, int first, int middle, int last);
extern void merge_sort(int array[], int first, int last);
extern void* malloc(uint32_t size);
extern void* calloc(uint32_t size);
extern void free(void* ptr);
extern void* realloc(void* ptr, uint32_t size_new);
extern void exit(void);
extern uint32_t rand(void);
xchar inputg(void);
char inputc(void);
uint32_t int_to_sectors(uint32_t num);

#ifdef __cplusplus
}
#endif



extern bool app_exited;
extern int last_command_exit_status;
extern uint8_t null_region[20];

typedef uint8_t* address_t;


enum XANIN_RETURN_STATUS
{
    XANIN_OK = 0x0,
    XANIN_ERROR = 0x3,
    XANIN_TOO_LONG_STRING = 0x4,
};
