
#include <lib/bmp/bmp.h>
#include <sys/flow/exit_codes.h>
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>

//TERMINAL_APP

__STATUS bmp_info(const char* filename)
{
    XinEntry* File = fopen(filename, "r");

    BitMapFileStructure BmpInfo;

    if(File == NULL)
    {
        xprintf("ERROR: Could not open\n");
        getchar();
        return XANIN_ERROR;
    }

    fread(File, &BmpInfo, SIZE_OF(BitMapFileStructure));

    xprintf("%zBitMapHeader:\n", OUTPUT_COLOR_SET(black, green));
    xprintf("signature: ");
     
    for(int i = 0; i < SIZE_OF(BmpInfo.Header.signature); i++)
        putchar(BmpInfo.Header.signature[i]);
    
    xprintf("\nfile_size: %d\n", BmpInfo.Header.file_size);
    xprintf("resv: %d\n", BmpInfo.Header.reserved);
    xprintf("data_offset: %d\n", BmpInfo.Header.data_offset);

    xprintf("%zBitMapInfoHeader:\n", OUTPUT_COLOR_SET(black, green));
    xprintf("size: %d\n", BmpInfo.InfoHeader.size);
    xprintf("width(px): %d\n", BmpInfo.InfoHeader.width);
    xprintf("height(px): %d\n", BmpInfo.InfoHeader.height);
    xprintf("planes: %d\n", BmpInfo.InfoHeader.planes);
    xprintf("bit_count: %d\n", BmpInfo.InfoHeader.bit_count);
    xprintf("compression: %d\n", BmpInfo.InfoHeader.compression);
    xprintf("image_size: %d\n", BmpInfo.InfoHeader.image_size);
    xprintf("x_pixels_per_m: %d\n", BmpInfo.InfoHeader.x_pixels_per_m);
    xprintf("y_pixels_per_m: %d\n", BmpInfo.InfoHeader.y_pixels_per_m);
    xprintf("colors_used: %d\n", BmpInfo.InfoHeader.colors_used);
    xprintf("colors_important: %d\n", BmpInfo.InfoHeader.colors_important);
    
    getchar();
    return XANIN_OK;
    
}