#pragma once

#include "../lib/stdiox.h"
#include "../headers/inttypes.h"
#include "../lib/stdlibx.h"
#include "../xaninGraphics/xaninGraphics.c"
#include "../lib/signal.h"


uint32_t** createEnemy(void);
void initSpace(void);


uint32_t* enemies[15];

uint32_t* nextEnemy = (uint32_t*)0xb8000 + 80;