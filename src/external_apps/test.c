
// #include "../sys/terminal/vty.h"

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

void main(void)
{

    char* tmp = (uint8_t*)malloc(100);

    xprintf("podaj tmp: ");
    xscanf("%s", tmp);
    xprintf("\ntwoje dane: %s\n", tmp);

    __sys_inputg();
    
    return;
}
