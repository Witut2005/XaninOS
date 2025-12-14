
#include <lib/bmp/bmp.h>
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>
#include <lib/xgl/xgl.h>
#include <sys/flow/exit_codes.h>

STATUS bmp_viewer(char const* filename)
{
    XinEntry* File = fopen(filename, "r");

    if (File == NULL)
    {
        xprintf("ERROR: Could not open %s\n", filename);
        return XANIN_ERROR;
    }

    uint8_t* buf = calloc(File->size);
    BitMapFileStructure* BmpInfo = (BitMapFileStructure*)buf;
    fread(File, buf, File->size);
    
    int width = BmpInfo->InfoHeader.width;
    int height = BmpInfo->InfoHeader.height;

    if (width != 320 || height != 200) {
        xprintf("BMP viewer only supports 320x200 bitmap size");
    }

    xgl_init(VGA_GRAPHICS_320x200x256, VGA_PALETTE_DEFAULT);
    uint8_t* vga_buf = (uint8_t*)vga_get_buffer_segment();
    
    int colors_to_load = BmpInfo->InfoHeader.colors_used == 0 ? 256 : BmpInfo->InfoHeader.colors_used;
    uint8_t *palette_start = buf + sizeof(BitMapHeader) + BmpInfo->InfoHeader.size;
    vga_load_bmp_palette(palette_start, colors_to_load);

    uint8_t* byte_array = buf + BmpInfo->Header.data_offset;
    
    for(int bmp_y = 0; bmp_y < height; bmp_y++) {
        int screen_y = (height - 1) - bmp_y; 
        
        for(int x = 0; x < width; x++) {
            uint32_t bmp_index = (uint32_t)bmp_y * width + x;
            
            uint32_t screen_linear_index = (uint32_t)screen_y * width + x;
            uint8_t color = byte_array[bmp_index];
            
            pixel_linear_set(screen_linear_index, color);
        }
    }

    getchar();
    return XANIN_OK;
}