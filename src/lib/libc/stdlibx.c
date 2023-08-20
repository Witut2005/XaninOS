
#include <lib/libc/hal.h>
#include <lib/libc/time.h>
#include <sys/terminal/vty/vty.h>
#include <lib/libc/colors.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <sys/devices/pit/pit.h>
#include <lib/libc/memory.h>
#include <lib/libc/process.h>

uint8_t null_region[20] = {0};

CmosTime SystemTime;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

float pit_time;

char* daysLUT[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
uint8_t* null_memory_region;

extern void kernel_loop(void);

int integer(void* value)
{
    return (int)value;
}

uint32_t int_to_sectors(uint32_t num)
{
    int size = num / SECTOR_SIZE;
    if(num % SECTOR_SIZE)
        size++;
    return size;
}

void exit(void)
{
    // eoi_send();
    // kernel_loop();
    
    asm("mov eax, 1\n\t"
        "int 0x80");

    app_process_unregister();
}


char getchar(void)
{
    char ret;

    asm("mov eax, 201;" // getchar syscall id
        "mov %0, al;"    
        "int 0x81;"
        : "=r"(ret)
    );

    return ret;
}

char getscan(void)
{
    char ret;

    asm("mov eax, 202;" // getchar syscall id
        "mov %0, al;"    
        "int 0x81;"
        : "=r"(ret)
    );

    return ret;
}

void __sys_inputg(xchar* ptr)
{
    asm("mov eax, 203;" // inputg syscall id
        "mov ecx, %0;"
        "int 0x81;"
        :
        : "g"(ptr) 
    );
}

void __sys_keyinfo_get(key_info_t* ptr)
{
    asm("mov eax, 204;" // inputg syscall id
        "mov ecx, %0;"
        "int 0x81;"
        :
        : "r"(ptr) 
    );
}

// void keyboard_buffer_refresh(uint16_t* screen_buffer)
// {
//     for(int i = 0; i < 50; i++)
//         string_typed_buffer[i] = *(char*)(screen_buffer + i);
// }

CmosTime* time_get(CmosTime* Time)
{
    interrupt_disable();


    //GET SECONDS
    outbIO(CMOS_ADDR,0x0);
    Time->seconds = inbIO(CMOS_DATA);

    //GET MINUTES
    outbIO(CMOS_ADDR,0x2);
    Time->minutes = inbIO(CMOS_DATA);

    //GET HOURS
    outbIO(CMOS_ADDR,0x4);
    Time->hour = inbIO(CMOS_DATA);

    
    if((((Time->hour & 0xF0) >> 8) == 2) && ((Time->hour & 0x0F) >= 2))
    {
        Time->hour &= 0x0F;
        Time->hour -= 2;

        Time->hour = Time->hour << 4;
    }

    else if((Time->hour & 0xF) == 9 || (Time->hour & 0xF) == 8)
    {
        Time->hour = (Time->hour & 0xF0) + (Time->hour & 0x1);
        Time->hour = Time->hour + (1 << 4);
    }

    else
        Time->hour+=2;


    if(Time->hour >= 0x24)
        Time->hour = 0;

    //GET day
    outbIO(CMOS_ADDR,0x6);
    Time->weekday = inbIO(CMOS_DATA);
    Time->weekday--;

    //GET DAY_OF_MONTH
    outbIO(CMOS_ADDR,0x7);
    Time->day_of_month = inbIO(CMOS_DATA);

    //GET MONTH
    outbIO(CMOS_ADDR,0x8);
    Time->month = inbIO(CMOS_DATA);

    //GET YEAR
    outbIO(CMOS_ADDR,0x9);
    Time->year = inbIO(CMOS_DATA);

    //GET CENTURY
    outbIO(CMOS_ADDR,0x32);
    Time->century = inbIO(CMOS_DATA);

    interrupt_enable();
    return Time;
}

uint8_t floppy_type_get_cmos(void)
{
    outbIO(CMOS_ADDR, 0x10);
    return inbIO(CMOS_DATA);
}


uint32_t randomizer;



void swap_int(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_char(char* xp, char* yp)
{
    char temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_short(uint16_t* xp, uint16_t* yp)
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
    int n1 = middle - first + 1;  //SIZE_OF
    int n2 = last - middle;        //SIZE_OF
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

int reboot(void)
{

    // xprintf("nicho");
    *(uint16_t*)VGA_TEXT_MEMORY = 0x4141;

    // screen_clear();
    uint16_t idt_16[3] = {0x0, 0x1234, 0x4567};

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 20, (uint16_t*)(0x1800));

    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    return XANIN_OK;

}

bool app_exited = false;

IntervalEntry XaninIntervals[INTERVALS_MAX] = {INTERVAL_CLEAR};

interval_id interval_set(interval_handler handler, float ms, address_t* args)
{

    for(int i = 0; i < INTERVALS_MAX; i++)
    {
        if(!XaninIntervals[i].is_in_use)
        {
            XaninIntervals[i].is_in_use = INTERVAL_IN_USE;
            XaninIntervals[i].handler = handler;
            XaninIntervals[i].arguments = args;
            XaninIntervals[i].timeout = ms / 1000;
            XaninIntervals[i].start_time = 0;
            return (interval_id)i;
        }
    }
    return INTERVAL_CANT_INIT;
}

void interval_clear(interval_id used_interval)
{
    XaninIntervals[used_interval].is_in_use = INTERVAL_CLEAR;
}

void all_intervals_clear(void)
{
    for(int i = 0; i < INTERVALS_MAX; i++)
        XaninIntervals[i].is_in_use = INTERVAL_CLEAR;
}

void do_interval(interval_id interval)
{
    if(XaninIntervals[interval].is_in_use)
    {
        if(pit_time > (XaninIntervals[interval].start_time + XaninIntervals[interval].timeout)) // timeout reached
        {
            XaninIntervals[interval].start_time = pit_time;
            XaninIntervals[interval].handler(XaninIntervals[interval].arguments);
        }
    }
}