
#pragma once 

#include <stdint.h>
#include <stddef.h>

struct xchar
{
    char character;
    uint8_t scan_code;
};

typedef struct xchar xchar;

extern void screen_clear(void);
extern char putchar(char character);
extern void xprintf(char* str, ...);
extern void xscanf(char* str, ...);
extern xchar inputg(void);