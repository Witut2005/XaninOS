#include <stdint.h>

static uint32_t randomizer;

// https://crypto.stackexchange.com/questions/6760/how-does-the-rand-function-in-c-work
void srand(uint32_t seed)
{
    randomizer = seed;
}

uint32_t rand(void)
{
    return (((randomizer = randomizer * 214013 + 2531011) >> 16) & 0x7fff);
}
