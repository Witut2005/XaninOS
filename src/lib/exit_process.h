
#pragma once

#include <terminal/vty.h>
#include <terminal/vty.c>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>
#include <lib/stdiox.h>
#include <stddef.h>

#define exit_process(x)\
    index = 0x0;\
    no_enter = false;\
    for(int i = 0; i < sizeof(comBuf);i++)\
        keyboard_command[i] = '\0';\
        app_exited = true;\
    return x;

