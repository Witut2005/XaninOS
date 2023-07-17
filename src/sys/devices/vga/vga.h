
#pragma once
#include <stdint.h>


enum VGA_REGISTERS
{

    VGA_MISC_REGISTER =  0x3c2,
    VGA_CRTC_INDEX_REGISTER = 0x3d4,
    VGA_CRTC_DATA_REGISTER = 0x3d5,
    
    VGA_SEQUENCER_INDEX_REGISTER = 0x3c4,
    VGA_SEQUENCER_DATA_REGISTER = 0x3c5,

    VGA_GRAPHICS_INDEX_REGISTER = 0x3ce,
    VGA_GRAPHICS_DATA_REGISTER = 0x3cf,

    VGA_ATTRIBUTE_INDEX_REGISTER = 0x3c0,
    VGA_ATTRIBUTE_READ_REGISTER = 0x3c1,
    VGA_ATTRIBUTE_WRITE_REGISTER = 0x3c0,
    VGA_ATTRIBUTE_RESET_REGISTER = 0x3da,

	VGA_MISC_OUTPUT_REGISTER = 0x3C2,
	VGA_GRAPHICS_MODE_REGISTER = 0x3CE

};

enum VGA_STATES
{
    VGA_TEXT_90x60,
    VGA_TEXT_80x25,
    VGA_GRAPHICS_320x200x4, 
    VGA_GRAPHICS_640x480x16,
    VGA_GRAPHICS_720x480x16,
	VGA_GRAPHICS_320x200x256,
    VGA_GRAPHICS_320x200x256_MODEX
};

typedef uint8_t xgm_t;


//these resources helped me a lot!!!
//https://www.youtube.com/watch?v=N68cYNWZgy8&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M&index=13
//https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c




// extern unsigned char g_90x60_text[];
// extern unsigned char g_80x25_text[];
// extern unsigned char g_720x480x16[];
// extern unsigned char g_320x200x256[];
// extern unsigned char g_8x8_font[];
// extern unsigned char g_8x16_font[];

#ifdef __cplusplus
extern "C" {
#endif

void vga_registers_write(uint8_t* registers);
void set_plane(unsigned p);
unsigned get_fb_seg(void);
void vmemwr(unsigned dst_off, unsigned char *src, unsigned count);
void vga_font_write(char *buf, unsigned font_height);
unsigned char readRegVGA(unsigned short reg, unsigned char idx);
void writeRegVGA(unsigned short reg, unsigned char idx, unsigned char val);
void setFontVGA(const unsigned char * buffer, int h);
void vga_mode_set(uint8_t vga_mode);
uint8_t* vga_get_buffer_segment(void);


#ifdef __cplusplus
}
#endif

