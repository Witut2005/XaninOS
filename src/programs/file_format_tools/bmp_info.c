
#include <lib/bmp/bmp.h>
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>
#include <lib/xgl/xgl.h>
#include <sys/flow/exit_codes.h>

STATUS bmp_info(char const* filename)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    // ... (kod wczytywania i sprawdzania nagłówków) ...
    XinEntry* File = fopen(filename, "r");

    uint8_t* buf = calloc(File->size);
    BitMapFileStructure* BmpInfo = (BitMapFileStructure*)buf;

    if (File == NULL)
    {
        xprintf("ERROR: Could not open\n");
        getchar();
        return XANIN_ERROR;
    }

    fread(File, buf, File->size);


    xgl_init(VGA_GRAPHICS_320x200x256, VGA_PALETTE_DEFAULT);
    
    uint8_t* vga_buf = (uint8_t*)vga_get_buffer_segment();
    
    int colors_to_load = BmpInfo->InfoHeader.colors_used == 0 ? 256 : BmpInfo->InfoHeader.colors_used;
    uint8_t *palette_start = buf + sizeof(BitMapHeader) + BmpInfo->InfoHeader.size;
    vga_load_bmp_palette(palette_start, colors_to_load);
    
    // --- Krok 1: Załadowanie Palety ---
    
    // Zakładamy, że masz zaimplementowaną i używasz funkcji vga_load_bmp_palette
    // W przeciwnym razie kolory będą złe.

    // --- Krok 2: Rysowanie z korekcją Bottom-Up ---

    int width = BmpInfo->InfoHeader.width;
    int height = BmpInfo->InfoHeader.height;

    if (width != 320 || height != 200) {
        // ... (obsługa błędu)
    }

    // Pamiętaj: dane bitmapy są pod adresem BmpInfo->Header.data_offset
    uint8_t* byte_array = buf + BmpInfo->Header.data_offset;
    
    // Pętla od 0 do 199 (indeks w buforze BMP)
    for(int bmp_y = 0; bmp_y < height; bmp_y++) {
        // Odwrócenie Y: mapowanie dolnego wiersza BMP (bmp_y=0) na górny wiersz ekranu (screen_y=199)
        int screen_y = (height - 1) - bmp_y; 
        
        for(int x = 0; x < width; x++) {
            // Indeks do odczytu z wczytanego bufora BMP
            uint32_t bmp_index = (uint32_t)bmp_y * width + x;
            
            // Indeks do zapisu w pamięci liniowej VRAM
            uint32_t screen_linear_index = (uint32_t)screen_y * 320 + x;
            
            uint8_t color = byte_array[bmp_index];
            
            // Wypisz piksel, używając indeksu z BMP bez przesunięcia (+16)
            pixel_linear_set(screen_linear_index, color);
        }
    }

    getchar();
    return XANIN_OK;
}