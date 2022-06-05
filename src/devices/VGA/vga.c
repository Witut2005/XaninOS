
#include <devices/VGA/vga.h>
#include <libc/memory.h>

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

    // CRTC

    outbIO(VGA_CRTC_INDEX_REGISTER, 0x3);
    outbIO(VGA_CRTC_INDEX_REGISTER, inbIO(VGA_CRTC_DATA_REGISTER) | 0x80);
    
    outbIO(VGA_CRTC_INDEX_REGISTER, 0x11);
    outbIO(VGA_CRTC_INDEX_REGISTER, inbIO(VGA_CRTC_DATA_REGISTER) &(~0x80));

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for(int i = 0; i < 25; i++, registers++)
    {
        outbIO(VGA_CRTC_INDEX_REGISTER, i);
        outbIO(VGA_CRTC_DATA_REGISTER, *registers); 
    }

    for(int i = 0; i < 9; i++, registers++)
    { 
        outbIO(VGA_GRAPHICS_INDEX_REGISTER, i);
        outbIO(VGA_GRAPHICS_DATA_REGISTER, *registers);
    }

    for(int i = 0; i < 21; i++, registers++)
    {
        inbIO(VGA_ATTRIBUTE_RESET_REGISTER);
        outbIO(VGA_ATTRIBUTE_INDEX_REGISTER, i);
        outbIO(VGA_ATTRIBUTE_WRITE_REGISTER, *registers);
    }

    inbIO(VGA_ATTRIBUTE_RESET_REGISTER);
    outbIO(VGA_ATTRIBUTE_INDEX_REGISTER, 0x20);

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

        
        case VGA_TEXT_90x60:
        { 
            vga_registers_write(g_90x60_text);
            setFontVGA(g_8x8_font, 8);
            break;
        }
        
        case VGA_TEXT_80x25:
        {
            vga_registers_write(g_80x25_text);
            setFontVGA(g_8x16_font, 16);
            break;
        }

        case VGA_GRAPHICS_720x480x16:
        {
            vga_registers_write(g_720x480x16);
            break;
        }

    }



}
