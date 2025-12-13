#include "./bmp.h"

BitMapHeader* bmp_header_get(XinEntry* bmp, BitMapHeader* buf) {
    fread(bmp, buf, sizeof(BitMapHeader));
    return buf;
}