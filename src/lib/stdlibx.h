
#pragma once

#include <lib/hal.h>
#include <pit/pit.c>
#include <lib/time.h>
#include <terminal/vty.h>
#include <headers/colors.h>
#include <handlers/handlers.c>

char command_buffer[50];
char* keyboard_command;

struct TIME
{
    uint8_t seconds; 
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekDay;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t year;
    uint8_t century;
    

}__attribute__((packed));

struct TIME time;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

char* weekDaysLUT[7] = {"Sunday","Monday","Tuesday ","Wednesday","Thursday","Friday","Saturday"};

bool key_pressed(void)
{
    if(key_released)
        return false;
    else
        return true;
}

char getchar(void)
{
    char tmp = KeyInfo.character;
    KeyInfo.character = 0x0;
    return tmp;
}

char getscan(void)
{
    uint8_t tmp = KeyInfo.scan_code;
    KeyInfo.scan_code = 0x0;
    return tmp;
}

char* keyboard_buffer_refresh(uint16_t* screen_buffer)
{
    for(int i = 0; i < 50; i++)
        keyboard_command[i] = *(char*)(screen_buffer + i);

}

void time_get()
{
    asm("cli");


    //GET SECONDS
    outbIO(CMOS_ADDR,0x0);
    time.seconds = inbIO(CMOS_DATA);

    //GET MINUTES
    outbIO(CMOS_ADDR,0x2);
    time.minutes = inbIO(CMOS_DATA);

    //GET HOURS
    outbIO(CMOS_ADDR,0x4);
    time.hour = inbIO(CMOS_DATA);

    //GET WEEKDAY
    outbIO(CMOS_ADDR,0x6);
    time.weekDay = inbIO(CMOS_DATA);
    time.weekDay--;

    //GET DAY_OF_MONTH
    outbIO(CMOS_ADDR,0x7);
    time.dayOfMonth = inbIO(CMOS_DATA);

    //GET MONTH
    outbIO(CMOS_ADDR,0x8);
    time.month = inbIO(CMOS_DATA);

    //GET YEAR
    outbIO(CMOS_ADDR,0x9);
    time.year = inbIO(CMOS_DATA);

    //GET CENTURY
    outbIO(CMOS_ADDR,0x32);
    time.century = inbIO(CMOS_DATA);

    asm("sti");
}



void get_cpu_speed()
{
    set_pit_divisor(0x0);

    while(!pitActive)
        cpu_mhz++;

    pitActive = false;
}

static uint32_t randomizer;



void swap_int(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_char(char* *xp, char* yp)
{
    char temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_short(uint16_t* *xp, uint16_t* yp)
{
    uint16_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}


//https://crypto.stackexchange.com/questions/6760/how-does-the-rand-function-in-c-work

void srand(uint32_t seed)
{
    randomizer = seed;
}


uint32_t rand()
{
    return (((randomizer = randomizer * 214013 + 2531011) >> 16) & 0x7fff);
}

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

reg_t Register;
seg_t SegmentRegister;



void int_swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubble_sort(int* array, int n)  //syntax bubble_sort(array[], n=size of array)
{
bool swapped;
int i, j;
  for (i=0; i<n-1; i++)
  {
      swapped = false;
      for(j=0; j<n-i-1; j++)
      {
        if(array[j]>array[j+1])
        {
            int_swap(&array[j], &array[j+1]);
            swapped = true;
        }
      }
    if (swapped == false)
        break;
  }

}

void merge(int* array, int first, int middle, int last)
{
    int i, j, k;
    int n1 = middle - first + 1;  //sizeof
    int n2 = last - middle;        //sizeof
    int L[n1], R[n2];

    for (i = 0; i<n1; i++)          //copy array into L
        L[i]=array[first+i];
    for (j = 0; j<n2; j++)
        R[j]=array[middle + 1 + j];

    i=0;
    j=0; 
    k=first;
    while (i<n1&&j<n2)
    {
        if(L[i]<=R[j])
        {
            array[k]=L[i];
            i++;
        }

        else
        {
            array[k]=R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }
}


void merge_sort(int array[], int first, int last)
{
    if(first<last)
    {
        int middle=first+(last-first)/2;

        merge_sort(array, first, middle);
        merge_sort(array, middle+1, last);

        merge(array, first, middle, last);
    }

}

uint16_t* memory_managament_map = (uint16_t*)0x20000; 

void bit_set(uint16_t* address, uint16_t bit_number)
{
    *address |= (1 << bit_number);
}

bool bit_clear(uint16_t* address, uint16_t bit_number)
{
    *address -= (1 << bit_number);
}

void memory_manager_init(void)
{
    for(char* i = 0x20000; (uint32_t)i <= 0x40000; i++)
        *i = 0x0;
}

void* malloc(uint16_t size)
{
    char* allocation_table = (char*)0x20000;

    //*allocation_table = 
}

void* free(uint16_t* addr)
{



}