
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

extern char command_buffer[80];
extern char* keyboard_command;

bool key_pressed(void);
void wait_key(uint8_t key);
char getchar(void);
char getscan(void);
char* keyboard_buffer_refresh(uint16_t* screen_buffer);
CmosTime* time_get(CmosTime* Time);
uint8_t floppy_type_get_cmos(void);
void get_cpu_speed(void);
static uint32_t randomizer;
void swap_int(int *xp, int *yp);
void swap_char(char* xp, char* yp);
void swap_short(uint16_t* xp, uint16_t* yp);
void srand(uint32_t seed);
// void (*abort_return_address)(void);

uint32_t rand(void);

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

uint32_t memory_map_get_cmos(void);
void int_swap(int *xp, int *yp);
void bubble_sort(int* array, int n);  //syntax bubble_sort(array[], n=size of array)
void merge(int* array, int first, int middle, int last);
void merge_sort(int array[], int first, int last);
void* malloc(uint16_t size);
void free(void* ptr);
void* realloc(void* ptr,  uint32_t size_old,  uint32_t size_new);

struct xchar
{
    char character;
    uint8_t scan_code;
};

typedef struct xchar xchar;

xchar inputg(void);

extern bool app_exited;