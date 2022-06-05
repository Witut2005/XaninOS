
#pragma once

#include <stdint.h>
#include <stddef.h>

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

extern bool key_pressed(void);
extern void wait_key(uint8_t key);
extern char getchar(void);
extern char getscan(void);
extern char* keyboard_buffer_refresh(uint16_t* screen_buffer);
extern CmosTime* time_get(CmosTime* Time);
extern uint8_t floppy_type_get_cmos(void);
extern void get_cpu_speed(void);
extern void swap_int(int* xp, int* yp);
extern void swap_char(char* xp, char* yp);
extern void swap_short(uint16_t* xp, uint16_t yp);
extern void srand(uint32_t seed);
extern uint32_t rand(void);
extern uint32_t memory_map_get_cmos(void);
extern void int_swap(int* xp, int* yp);
extern void bubble_sor(int* array, int n);
extern void marge(int* array, int first, int middle, int last);
extern void merge_sort(int* array, int first, int last);
extern void* malloc(uint16_t size);
extern void free(void* ptr);
extern void* realloc(void* ptr, uint32_t size_old, uint32_t size_new);
extern uint8_t* xanin_information_block_get(void);