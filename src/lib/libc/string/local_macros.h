
#pragma once

#define LIBC_STR_EXIT_ON_EQUALS_ZERO(val, return_value) \
    if (val == 0) { return return_value; }
