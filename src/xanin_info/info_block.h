
#pragma once

#include <stdint.h>

struct XaninInfoBlock
{

    char signature[10];
    uint32_t program_to_execute;

};

#ifndef __cplusplus
typedef struct XaninInfoBlock XaninInfoBlock;
XaninInfoBlock* xanin_information_block_get(void);
#endif
static XaninInfoBlock* xanin_info_ptr;