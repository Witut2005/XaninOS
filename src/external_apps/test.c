
// #include "../sys/terminal/vty.h"

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

struct screen_t
{
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;

};

typedef struct screen_t screen_t;

extern screen_t Screen;
extern void* calloc(uint32_t size);
extern void xscanf(char* str, ... );


static char tmp[100];

void main(int argc, char* argv[])
{
    // char* tmp = (uint8_t*)calloc(100);
    memset(tmp, 0, 100);
    xprintf("tmp 0x%x\n", tmp);

    // xprintf("\ntwoje dane: %s\n", tmp);
    // xprintf("podaj tmp: ");
    // memset(tmp, 0, 100);
    xscanf("%s", tmp);
    
    xprintf("twoje dane: %s\n", tmp);
    // xprintf("tmp 0x%x\n", tmp);

    __sys_inputg();
    
    return;
}
