
#pragma once

#include <stdint.h>

struct XaninInfoBlock
{

    char signature[11];
    uint32_t program_to_execute;

};

#ifndef __cplusplus
typedef struct XaninInfoBlock XaninInfoBlock;
XaninInfoBlock* xanin_information_block_get(void);
static XaninInfoBlock* xanin_info_ptr;
#endif