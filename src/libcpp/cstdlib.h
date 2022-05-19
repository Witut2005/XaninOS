
#pragma once
#include <stdint.h>

extern "C"
{

bool key_pressed(void);
char getchar(void);
char getscan(void);
char* keyboard_buffer_refresh(uint16_t* screen_buffer);
void time_get();
uint8_t floppy_type_get_cmos();
void get_cpu_speed();
void swap_int(int *xp, int *yp);
void swap_char(char* *xp, char* yp);
void swap_short(uint16_t* *xp, uint16_t* yp);
void srand(uint32_t seed);
uint32_t rand();
uint32_t memory_map_get_cmos();
void int_swap(int *xp, int *yp);
void bubble_sort(int* array, int n) ;
void merge(int* array, int first, int middle, int last);
void merge_sort(int array[], int first, int last);
void bit_set(uint16_t* address, uint16_t bit_number);
bool bit_clear(uint16_t* address, uint16_t bit_number);
void memory_manager_init(void);
void* malloc(uint16_t size);
void* free(uint16_t* addr);
void wait_key(uint8_t key);

}