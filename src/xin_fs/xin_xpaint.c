

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <libc/string.h>

uint16_t saved_cell;
uint16_t* cursor;

void xpaint_input(void)
{
    
    if(!KeyInfo.scan_code)
        return;
    
    if(KeyInfo.scan_code >= 128)
        return;

    *cursor = saved_cell;

    if(KeyInfo.scan_code == ARROW_RIGHT)
    {
        cursor++;
    }

    else if(KeyInfo.scan_code == ARROW_LEFT)
    {
        cursor--;
    }

    else if(KeyInfo.scan_code == ARROW_DOWN)
    {
        cursor = cursor + 80;
    }

    else if(KeyInfo.scan_code == ARROW_UP)
    {
        cursor = cursor - 80;
    }

    if(KeyInfo.is_ctrl)
    {
        if(KeyInfo.character)
        {
            uint8_t color = KeyInfo.character;

            if(color >= 'a' && color <= 'f')
                color = color - 'a' + 0xa;   
            else if(color >= '0' && color <= '9')
                color = color - '0';
            else 
                return;

            uint8_t font_color = (*cursor >> 8) & 0xF;
            color = color << 4;

            *cursor = (uint16_t)(((color | font_color) << 8) | (*cursor & 0xFF));
        }
    }
    
    else if(KeyInfo.is_alt)
    {
        if(KeyInfo.character)
        {
            uint8_t color = KeyInfo.character;

            if(color >= 'a' && color <= 'f')
                color = color - 'a' + 0xa;   
            else if(color >= '0' && color <= '9')
                color = color - '0';
            else 
                return;

            uint8_t background_color = (*cursor >> 12) & 0xF;
            *cursor = (uint16_t)((color | background_color) << 8 | (*cursor & 0xFF));
        }
    }
    
    else if(KeyInfo.scan_code == DELETE_KEY)
    {
        *cursor = 0;
    }

    else
    {
        if(KeyInfo.character)
        {
        
            uint8_t color;
        
            if(!(*cursor && 0xFF00))
                color = set_output_color(black, white);
            else
                color = (*cursor & 0xFF00) >> 8;
            
            *cursor = (uint16_t)(color << 8 | KeyInfo.character);
        }
    }
    


    saved_cell = *cursor;

    if(!KeyInfo.character)
        *cursor = (set_output_color(black, white) << 8) | '_';

    KeyInfo.scan_code = 0x0;
}


int xin_xpaint(char* file_name)
{

    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else
    {
    
        uint16_t* data_pointer = (uint16_t*)calloc(VGA_SCREEN_RESOLUTION);
        read(xin_file, data_pointer, VGA_SCREEN_RESOLUTION);
        screen_clear();

        uint16_t* screen_cell = (uint16_t*)VGA_TEXT_MEMORY;

        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
            screen_cell[i] = (uint16_t) (data_pointer[i]);

        cursor = (uint16_t*)VGA_TEXT_MEMORY;
        saved_cell = *cursor;

        while(KeyInfo.scan_code != F4_KEY_RELEASE)
            xpaint_input();
        *cursor = saved_cell;
        fseek(xin_file, XIN_FILE_BEGIN);
        write(xin_file, (char*)VGA_TEXT_MEMORY, VGA_SCREEN_RESOLUTION);
    }
    fclose(&xin_file);
    return XANIN_OK;

}

// 21:16:21
// 19:51:50