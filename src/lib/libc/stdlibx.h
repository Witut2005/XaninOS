
#pragma once

#include <fs/xin.h>
#include <lib/libc/system.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/call/xanin_sys/calls/input/input.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/input/key_info.h>
#include <sys/macros.h>

#define XANIN_PMMNGR_BLOCK_SIZE 4096
#define ARRAY_LENGTH(x) (SIZE_OF(x) / SIZE_OF(x[0]))

extern void (*keyboard_handle)(void);

typedef uint8_t* address_t;
typedef uint8_t* object_t;

typedef uint32_t bcd_date_t;
typedef uint16_t bcd_time_t;

struct CmosTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;

    uint8_t weekday;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t century;

} __attribute__((packed));

typedef struct CmosTime CmosTime;
extern CmosTime SystemTime;

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

extern char* daysLUT[7];

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
} typedef reg_t;

struct
{
    uint16_t cs;
    uint16_t es;
    uint16_t ds;
    uint16_t ss;
    uint16_t fs;
    uint16_t gs;
} typedef seg_t;

extern reg_t Register;
extern seg_t SegmentRegister;

#ifdef __cplusplus
extern "C" {
#endif

    extern float pit_time;

    bcd_date_t time_extern_date(CmosTime* Time);
    bcd_time_t time_extern_time(CmosTime* Time);
    bool key_pressed(void);
    char getchar(void);
    char getscan(void);
    void keyboard_buffer_refresh(uint16_t* screen_buffer);
    CmosTime* time_get(CmosTime* Time);
    uint8_t floppy_type_get_cmos(void);
    void get_cpu_speed(void);
    void swap_int(int* xp, int* yp);
    void swap_char(char* xp, char* yp);
    void swap_short(uint16_t* xp, uint16_t* yp);
    void srand(uint32_t seed);
    uint32_t memory_map_get_cmos(void);
    void int_swap(int* xp, int* yp);
    void bubble_sort(int* array, int n);
    void merge(int* array, int first, int middle, int last);
    void merge_sort(int array[], int first, int last);

    static inline void* malloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_ALLOCATE, (uint32_t)size); }
    static inline void* calloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_CALLOCATE, (uint32_t)size); }
    static inline void free(void* ptr) { xanin_syscall1(XANIN_FREE, (uint32_t)ptr); }
    static inline void* realloc(void* ptr, uint32_t size) { return (void*)xanin_syscall2(XANIN_REALLOCATE, (uint32_t)ptr, (uint32_t)size); }

    // void* kmalloc(uint32_t size);
    // void* kcalloc(uint32_t size);
    // void kfree(void* ptr);
    // void* krealloc(void* ptr, uint32_t size_new);
    uint32_t int_to_sectors(uint32_t num);
    static inline uint32_t size_to_mmngr_blocks(uint32_t size) { return int_to_sectors(size); }

    void exit(void);
    uint32_t rand(void);
    int reboot(void);


    char getchar(void);
    char getscan(void);
    xchar getxchar(void);
    xchar inputg(void);
    key_info_t keyinfo_get(void);

#ifdef __cplusplus
}
#endif

extern bool app_exited;
extern uint8_t null_region[20];

typedef uint8_t* address_t;

enum XANIN_RETURN_STATUS {
    XANIN_OK = 0x0,
    XANIN_ERROR = 0x3,
    XANIN_TOO_LONG_STRING = 0x4
};

enum XANIN_INTERVAL {
    INTERVALS_MAX = 5,
    INTERVAL_CLEAR = 0,
    INTERVAL_IN_USE = 1,
    INTERVAL_CANT_INIT = 0xFFFFFFFF
};

typedef uint32_t interval_id;
typedef void (*interval_handler)(address_t* args);

struct IntervalEntry {
    bool is_in_use;
    interval_handler handler;
    address_t* arguments;
    float timeout;
    float start_time;
};

typedef struct IntervalEntry IntervalEntry;
extern IntervalEntry XaninIntervals[INTERVALS_MAX];

#ifdef __cplusplus
extern "C" {
#endif

    interval_id interval_set(interval_handler handler, float ms, address_t* args);
    void interval_clear(interval_id used_interval);
    void do_interval(interval_id interval);
    void all_intervals_clear(void);

#ifdef __cplusplus
}
#endif
