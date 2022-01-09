#include <stdint.h>

struct stack
{
    void(*popPtr)(void);
};

uint16_t stackPtr;