

#include <devices/VGA/vga.h>
#include <libc/memory.h>


#define VGA_CRTC_LOCK() outbIO(VGA_CRTC_INDEX_REGISTER, 0x3); \
                    outbIO(VGA_CRTC_DATA_REGISTER, inbIO(VGA_CRTC_DATA_REGISTER) | 0x80)

#define VGA_CRTC_UNLOCK() outbIO(VGA_CRTC_INDEX_REGISTER, 0x11); \
                outbIO(VGA_CRTC_DATA_REGISTER, inbIO(VGA_CRTC_DATA_REGISTER) &(~0x80))

unsigned char g_90x60_text[] =
{
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x03, 0x00, 0x02,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00,
};


unsigned char g_80x25_text[] =
{
/* MISC */
	0x67,
/* SEQ */
	0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
	0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};

unsigned char g_720x480x16[] =
{
/* MISC */
	0xE7,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x40, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x01, 0x00, 0x0F, 0x00, 0x00,
};


unsigned char g_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};


void vga_registers_write(uint8_t* registers)
{

    // MISC
    outbIO(VGA_MISC_REGISTER, *registers);
    registers++;

    // SEQ
    for(int i = 0; i < 5; i++,registers++)
    {
        outbIO(VGA_SEQUENCER_INDEX_REGISTER, i);
        outbIO(VGA_SEQUENCER_DATA_REGISTER, *registers);
    }


    VGA_CRTC_UNLOCK();

    // registers[0x03] = registers[0x03] | 0x80;
    // registers[0x11] = registers[0x11] & ~0x80;

    // CRTC
    for(int i = 0; i < 25; i++, registers++)
    {
        outbIO(VGA_CRTC_INDEX_REGISTER, i);
        outbIO(VGA_CRTC_DATA_REGISTER, *registers); 
    }

    //VGA_GRAPHICS
    for(int i = 0; i < 9; i++, registers++)
    { 
        outbIO(VGA_GRAPHICS_INDEX_REGISTER, i);
        outbIO(VGA_GRAPHICS_DATA_REGISTER, *registers);
    }

    //VGA_)ATTRIBUTE
    for(int i = 0; i < 21; i++, registers++)
    {
        inbIO(VGA_ATTRIBUTE_RESET_REGISTER);
        outbIO(VGA_ATTRIBUTE_INDEX_REGISTER, i);
        outbIO(VGA_ATTRIBUTE_WRITE_REGISTER, *registers);
    }

    inbIO(VGA_ATTRIBUTE_RESET_REGISTER);
    outbIO(VGA_ATTRIBUTE_INDEX_REGISTER, 0x20);

    VGA_CRTC_LOCK();


}


void set_plane(unsigned p)
{

    unsigned char pmask;

    	p &= 3;
	  pmask = 1 << p;
    
    /* set read plane */
	  outbIO(VGA_GRAPHICS_INDEX_REGISTER, 4);
	  outbIO(VGA_GRAPHICS_DATA_REGISTER, p);
    
    /* set write plane */
	  outbIO(VGA_SEQUENCER_INDEX_REGISTER, 2);
	  outbIO(VGA_GRAPHICS_DATA_REGISTER, pmask);

}

unsigned get_fb_seg(void)
{
	unsigned seg;

	outbIO(VGA_GRAPHICS_INDEX_REGISTER, 6);
	seg = inbIO(VGA_GRAPHICS_DATA_REGISTER);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}

#define	_vmemwr(DS,DO,S,N)	memcpy((char *)((DS) * 16 + (DO)), S, N)

void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
{
	_vmemwr(get_fb_seg(), dst_off, src, count);
}

void vga_font_write(char *buf, unsigned font_height)
{

	  unsigned char seq2, seq4, gc4, gc5, gc6;
	  unsigned i;

    /* save registers
    set_plane() modifies GC 4 and SEQ 2, so save them as well */

    outbIO(VGA_SEQUENCER_INDEX_REGISTER, 2);
    seq2 = inbIO(VGA_SEQUENCER_DATA_REGISTER);

	outbIO(VGA_SEQUENCER_INDEX_REGISTER, 4);
	seq4 = inbIO(VGA_SEQUENCER_DATA_REGISTER);

    /* turn off even-odd addressing (set flat addressing)
    assume: chain-4 addressing already off */
    outbIO(VGA_SEQUENCER_DATA_REGISTER, 0x04);

    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 4);
    gc4 = inbIO(VGA_GRAPHICS_DATA_REGISTER);

    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 5);
    gc5 = inbIO(VGA_GRAPHICS_DATA_REGISTER);
    /* turn off even-odd addressing */
    outbIO(VGA_GRAPHICS_DATA_REGISTER, gc5 & ~0x10);

    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 6);
    gc6 = inbIO(VGA_GRAPHICS_DATA_REGISTER);
    /* turn off even-odd addressing */
    outbIO(VGA_GRAPHICS_DATA_REGISTER, gc6 & ~0x02);
    
    /* write font to plane P4 */
    set_plane(2);
    
    uint8_t* ptr = (unsigned char *) 0xB8000;
    int j;

    /* write font 0 */
    for(i = 0; i < 256; i++)
    {

        for(j = 0; j < font_height; j++)
        {
            ptr[32 * i + j] = buf[font_height * i + j];
        }
        
    }


    /* restore registers */
    outbIO(VGA_SEQUENCER_INDEX_REGISTER, 2);
    outbIO(VGA_SEQUENCER_DATA_REGISTER, seq2);
    outbIO(VGA_SEQUENCER_INDEX_REGISTER, 4);
    outbIO(VGA_SEQUENCER_DATA_REGISTER, seq4);
    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 4);
    outbIO(VGA_GRAPHICS_DATA_REGISTER, gc4);
    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 5);
    outbIO(VGA_GRAPHICS_DATA_REGISTER, gc5);
    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 6);
    outbIO(VGA_GRAPHICS_DATA_REGISTER, gc6);
    
}

#define	pokeb(S,O,V)		*(unsigned char *)(16uL * (S) + (O)) = (V)
#define	pokew(S,O,V)		*(unsigned short *)(16uL * (S) + (O)) = (V)




#define VGA_GFX_REG            0x3CE
#define VGA_SEQ_REG            0x3C4
#define VGA_CRT_REG            0x3D4

#define VGA_GFX_I_RESET         0x00
#define VGA_GFX_I_ENABLE      0x01
#define VGA_GFX_I_COLORCMP      0x02
#define VGA_GFX_I_ROTATE      0x03
#define VGA_GFX_I_READMAP      0x04
#define VGA_GFX_I_MODE         0x05
#define VGA_GFX_I_MISC         0x06
#define VGA_GFX_I_CNOCARE      0x07
#define VGA_GFX_I_BITMASK      0x08

#define VGA_SEQ_I_RESET         0x00
#define VGA_SEQ_I_CLOCK         0x01
#define VGA_SEQ_I_MAPMASK      0x02
#define VGA_SEQ_I_CHARMAP      0x03
#define VGA_SEQ_I_MEMMODE      0x04

unsigned char readRegVGA(unsigned short reg, unsigned char idx)
{
   outbIO(reg, idx);

   return inbIO(reg + 0x01);
}

void writeRegVGA(unsigned short reg, unsigned char idx, unsigned char val)
{
   outbIO(reg, idx);
   outbIO(reg + 1, val);
}

void setFontVGA(const unsigned char * buffer, int h)
{
   unsigned char seq2, seq4, gfx6;
   int i, j;
   unsigned char * mem;

   seq2 = readRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK, 0x04);

   seq4 = readRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE, 0x06);

   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_CHARMAP, 0x00);

   gfx6 = readRegVGA(VGA_GFX_REG, VGA_GFX_I_MISC);
   writeRegVGA(VGA_SEQ_REG, VGA_GFX_I_MISC, 0x00);

   mem = (unsigned char *) 0xB0000;

   for (i = 0; i < 256; i++)
   {
      for (j = 0; j < h; j++)
         mem[j] = buffer[h * i + j];

      mem += 32;
   }

   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_MISC, gfx6);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MEMMODE, seq4);
   writeRegVGA(VGA_SEQ_REG, VGA_SEQ_I_MAPMASK, seq2);

   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_MODE, 0x10);
   writeRegVGA(VGA_GFX_REG, VGA_GFX_I_BITMASK, 0xFF);
}



void vga_mode_set(uint8_t vga_mode)
{

    switch(vga_mode)
    {

        
        // case VGA_TEXT_90x60:
        // { 
        //     vga_registers_write(g_90x60_text);
        //     setFontVGA(g_8x8_font, 8);
        //     break;
        // }
        
        // case VGA_TEXT_80x25:
        // {
        //     vga_registers_write(g_80x25_text);
        //     setFontVGA(g_8x16_font, 16);
        //     break;
        // }

        case VGA_GRAPHICS_720x480x16:
        {
            vga_registers_write(g_720x480x16);
            break;
        }

        case VGA_GRAPHICS_320x200x256:
        {
            vga_registers_write(g_320x200x256);
            break;
        }

    }



}

uint8_t* vga_get_buffer_segment(void)
{
    outbIO(VGA_GRAPHICS_INDEX_REGISTER, 0x6);
    uint8_t segment = (inbIO(VGA_GRAPHICS_DATA_REGISTER) >> 2) & 0x3;

    switch(segment)
    {
        case 0: return (uint8_t*)0x0;
        case 1: return (uint8_t*)0xA0000;
        case 2: return (uint8_t*)0xB0000;
        case 3: return (uint8_t*)0xB8000;
    }

}
