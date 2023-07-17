

struct BitMapHeader {
    char signature[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
}__attribute__((packed));

struct BitMapInfoHeader {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes; // must be 1
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
}__attribute__((packed)); 

#ifndef __cplusplus
typedef struct BitMapHeader BitMapHeader;
typedef struct BitMapInfoHeader BitMapInfoHeader;
#endif


struct BitMapFileStructure
{
    BitMapHeader Header;
    BitMapInfoHeader InfoHeader;
}__attribute__((packed));

#ifndef __cplusplus
typedef struct BitMapFileStructure BitMapFileStructure;
#endif