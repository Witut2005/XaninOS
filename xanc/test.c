
#include <stdio.h>

#define MACRO(x) xprintf("%d", x)

void test()##__XANC_MODIFIER(MACRO(10))
{
}

void fro()##__XANC_MODIFIER(MACRO(10))
{
}

int main(void)
{
    test();
}