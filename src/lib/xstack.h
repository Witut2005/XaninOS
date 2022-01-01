#include "../headers/inttypes.h"

struct STACK
{
    void(*popPtr)(void);
};

uint16_t stackPtr;