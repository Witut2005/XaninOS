
#pragma once

#include <fs/xin.h>
#include <lib/libc/time.h>
#include <lib/system/input/input.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/input/key_info.h>
#include <sys/macros.h>

#include "./stdlibx/algo/algo.h"
#include "./stdlibx/file/file.h"
#include "./stdlibx/flow/flow.h"
#include "./stdlibx/random/random.h"

#define XANIN_PMMNGR_BLOCK_SIZE 4096
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))
#define STRING_CHARS(x) (int(sizeof(x) - sizeof('\0')))

typedef uint8_t* address_t;
typedef uint8_t* object_t;

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

#ifdef __cplusplus
extern "C" {
#endif

extern float pit_time;

bcd_date_t time_extern_date(CmosTime* Time);
bcd_time_t time_extern_time(CmosTime* Time);
char getchar(void);
char getscan(void);
CmosTime* time_get(CmosTime* Time);

static inline void* malloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_ALLOCATE, (uint32_t)size); }
static inline void* calloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_CALLOCATE, (uint32_t)size); }
static inline void free(void* ptr) { xanin_syscall1(XANIN_FREE, (uint32_t)ptr); }
static inline void* realloc(void* ptr, uint32_t size) { return (void*)xanin_syscall2(XANIN_REALLOCATE, (uint32_t)ptr, (uint32_t)size); }

uint32_t int_to_sectors(uint32_t num);
static inline uint32_t size_to_mmngr_blocks(uint32_t size) { return int_to_sectors(size); }

char getchar(void);
char getscan(void);
xchar getxchar(void);
xchar inputg(void);

#ifdef __cplusplus
}
#endif

extern bool app_exited;
extern uint8_t null_region[20];

enum XANIN_RETURN_STATUS {
    XANIN_OK = 0x0,
    XANIN_ERROR = 0x3,
    XANIN_TOO_LONG_STRING = 0x4
};
