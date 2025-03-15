
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
#include "./stdlibx/alloc/alloc.h"
#include "./stdlibx/file/file.h"
#include "./stdlibx/flow/flow.h"
#include "./stdlibx/random/random.h"

#define XANIN_PMMNGR_BLOCK_SIZE 4096
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))
#define STRING_CHARS(x) (int(sizeof(x) - sizeof('\0')))

typedef uint8_t* address_t;
typedef uint8_t* object_t;

extern char* keyboard_command;
extern uint8_t* null_memory_region;

#ifdef __cplusplus
extern "C" {
#endif

    char getchar(void);
    char getscan(void);

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
