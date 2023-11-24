
#pragma once

#include <stdint.h>

struct PairUint32 {
    uint32_t first;
    uint32_t second;
};

struct PairInt32 {
    int32_t first;
    int32_t second;
};

//----------------------------------------------------------------//

struct PairUint16 {
    uint16_t first;
    uint16_t second;
};

struct PairInt16 {
    int16_t first;
    int16_t second;
};

//----------------------------------------------------------------//

struct PairUInt8 {
    uint8_t first;
    uint8_t second;
};

struct PairInt8 {
    int8_t first;
    int8_t second;
};

#ifndef __cplusplus

typedef struct PairUInt32 PairUInt32;
typedef struct PairInt32 PairInt32;
typedef struct PairUInt16 PairUInt16;
typedef struct PairInt16 PairInt16;
typedef struct PairUInt8 PairUInt8;
typedef struct PairInt8 PairInt8;

#endif
