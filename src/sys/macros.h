
#pragma once

#define SIZE_OF_POINTED_TYPE(MACRO_PTR) SIZE_OF(*(MACRO_PTR))
#define SIZE_OF(x) sizeof(x)
#define XANIN_DEBUG_RETURN(RETURN_VALUE) return RETURN_VALUE