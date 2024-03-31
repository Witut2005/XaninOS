#include "./shared.h"


void main(int argc, char* argv[])
{
    // printf("my add %d\n", myadd(1, 5));
    myadd(10, 5);
    return;
}

void _start(void)
{
    main(0, (char**)0x0);
}