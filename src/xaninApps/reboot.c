
#pragma once

#include <stdint.h>
#include <lib/hal.h>

void reboot(void)
{


    real_mode_enter(0x1000, 0x400);


}