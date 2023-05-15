
#pragma once

#include <stdbool.h>

#ifdef __cplusplus
    extern "C"{
#endif
    extern void printk(const char * str);
    extern void syslog_disable(void);
    extern void syslog_enable(void);
    extern bool is_syslog_enabled(void);

#ifdef __cplusplus
    }
#endif