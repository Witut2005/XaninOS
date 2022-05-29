
#pragma once

#include <lib/hal.h>
#include <pit/pit.c>
#include <lib/time.h>
#include <terminal/vty.h>
#include <lib/colors.h>
#include <handlers/handlers.c>
#include <lib/alloc.h>
#include <lib/string.h>

char command_buffer[50];
char* keyboard_command;

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

CmosTime Time;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

char* daysLUT[7] = {"Sun","Mon","Tue ","Wed","Thu","Fri","Sat"};

bool key_pressed(void)
{
    if(key_released)
        return false;
    else
        return true;
}

void wait_key(uint8_t key)
{
    KeyInfo.scan_code = 0x0;
    while(1)
    {
        if(KeyInfo.scan_code == key)
            break;
    }
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

void time_get(void)
{
    asm("cli");


    //GET SECONDS
    outbIO(CMOS_ADDR,0x0);
    Time.seconds = inbIO(CMOS_DATA);

    //GET MINUTES
    outbIO(CMOS_ADDR,0x2);
    Time.minutes = inbIO(CMOS_DATA);

    //GET HOURS
    outbIO(CMOS_ADDR,0x4);
    Time.hour = inbIO(CMOS_DATA);
    
    if((Time.hour & 0xF0) == 2 && (Time.hour & 0x0F) >= 2)
    {
        Time.hour &= 0x0F;
        Time.hour -= 2;

        Time.hour = Time.hour << 4;

    }

    else if((Time.hour & 0xF) == 9 || (Time.hour & 0xF) == 8)
    {
        Time.hour = (Time.hour & 0xF0) + (Time.hour & 0x1);
        Time.hour = Time.hour + (1 << 4);
    }

    else
        Time.hour+=2;

    //GET day
    outbIO(CMOS_ADDR,0x6);
    Time.weekday = inbIO(CMOS_DATA);
    Time.weekday--;

    //GET DAY_OF_MONTH
    outbIO(CMOS_ADDR,0x7);
    Time.day_of_month = inbIO(CMOS_DATA);

    //GET MONTH
    outbIO(CMOS_ADDR,0x8);
    Time.month = inbIO(CMOS_DATA);

    //GET YEAR
    outbIO(CMOS_ADDR,0x9);
    Time.year = inbIO(CMOS_DATA);

    //GET CENTURY
    outbIO(CMOS_ADDR,0x32);
    Time.century = inbIO(CMOS_DATA);

    asm("sti");
}

uint8_t floppy_type_get_cmos(void)
{
    outbIO(CMOS_ADDR, 0x10);
    return inbIO(CMOS_DATA);
}

void get_cpu_speed(void)
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

void (*abort_return_address)(void);


uint32_t rand(void)
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

reg_t Register;
seg_t SegmentRegister;


uint32_t memory_map_get_cmos(void)
{
    outbIO(0x70, 0x30);
    uint32_t low_memory = inbIO(0x71);

    outbIO(0x70, 0x31);
    uint32_t high_memory = inbIO(0x71);

    return low_memory | (high_memory << 8);

}


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

void* malloc(uint16_t size)
{
    uint8_t* ptr = (uint8_t*)pmmngr_alloc_block();

    for(int i = 1; i < size / 4096; i++)
        pmmngr_alloc_block();

    return ptr;

}


void free(void* ptr)
{
    pmmngr_free_block(ptr);
}

void* realloc(void* ptr,  uint32_t size_old,  uint32_t size_new)
{

    if(size_old % 4096)
        size_old += 4096;

    if(size_new % 4096)
        size_new += 4096;

    char* ptr_old = ptr;

    pmmngr_free_block(ptr_old);

    for(uint32_t i = 1; i < size_old / 4096; i++)
    {
        pmmngr_free_block(ptr_old + (i * 4096));
    }


    ptr = pmmngr_alloc_block(ptr_old);

    for(uint32_t i = 1; i < size_new / 4096; i++)
    {
        pmmngr_alloc_block(ptr + (i * 4096));
    }

    return ptr;

}


uint8_t* xanin_information_block_get(void)
{   
    char* tmp = (char*)0x7C00;
    while(!strcmp(tmp, "XANIN_INFO")){tmp++;};

    return tmp;


}









