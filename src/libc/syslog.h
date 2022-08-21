
#pragma once


#ifdef __cplusplus
    extern "C" void printk(const char * str);
#else
    extern void printk(const char * str);
#endif