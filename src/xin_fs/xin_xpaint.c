

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <libc/string.h>

uint16_t saved_cell;
uint16_t* cursor;

void xpaint_input(xchar Input)
{
    
    if(!Input.scan_code)
        return;
    
    if(Input.scan_code >= 128)
        return;

    *cursor = saved_cell;

    if(Input.scan_code == ARROW_RIGHT)
    {
        cursor++;
    }

    else if(Input.scan_code == ARROW_LEFT)
    {
        cursor--;
    }

    else if(Input.scan_code == ARROW_DOWN)
    {
        cursor = cursor + VGA_WIDTH;
    }

    else if(Input.scan_code == ARROW_UP)
    {
        cursor = cursor - VGA_WIDTH;
    }

    if(KeyInfo.is_ctrl)
    {
        if(Input.character)
        {
            uint8_t background_color = Input.character;

            if(background_color >= 'a' && background_color <= 'f')
                background_color = background_color - 'a' + 0xa;   
            else if(background_color >= '0' && background_color <= '9')
                background_color = background_color - '0';
            else 
                return;

            uint8_t font_color = (*cursor >> 8) & 0xF;
            SCREEN_CELL_SET_FROM_POINTER(cursor, background_color, font_color, *cursor);
        }
    }
    
    else if(KeyInfo.is_alt)
    {
        if(Input.character)
        {
            uint8_t font_color = Input.character;

            if(font_color >= 'a' && font_color <= 'f')
                font_color = font_color - 'a' + 0xa;   
            else if(font_color >= '0' && font_color <= '9')
                font_color = font_color - '0';
            else 
                return;

            uint8_t background_color = ((*cursor >> 12) & 0xF);
            SCREEN_CELL_SET_FROM_POINTER(cursor, background_color, font_color, *cursor);
        }
    }
    
    else if(Input.scan_code == DELETE_KEY)
    {
        *cursor = (uint16_t)NULL;
    }

    else
    {
        if(Input.character)
        {
        
            uint8_t color;
        
            if(!(*cursor && 0xFF00))
                color = OUTPUT_COLOR_SET(black, white);
            else
                color = (*cursor & 0xFF00) >> 8;
            
            *cursor = (uint16_t)(color << 8 | Input.character);
        }
    }
    

    saved_cell = *cursor;

    if(!Input.character)
        *cursor = (OUTPUT_COLOR_SET(black, white) << 8) | '_';

}


int xin_xpaint(char* file_name)
{

    XinEntry* xin_file = fopen(file_name, "rw");

    if(xin_file == NULL)
    {
        xprintf("Could not open file %s\n", file_name);
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    else
    {
    
        uint16_t* data_pointer = (uint16_t*)calloc(VGA_SCREEN_RESOLUTION);
        fread(xin_file, data_pointer, VGA_SCREEN_RESOLUTION);
        screen_clear();

        uint16_t* screen_cell = (uint16_t*)VGA_TEXT_MEMORY;

        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
            screen_cell[i] = (uint16_t) (data_pointer[i]);

        cursor = (uint16_t*)VGA_TEXT_MEMORY;
        saved_cell = *cursor;

        while(true)
        {
            xchar Input = inputg();
            if(Input.scan_code == F4_KEY)
                break;
            xpaint_input(Input);
        }

        *cursor = saved_cell;
        fseek(xin_file, XIN_FILE_BEGIN);
        fwrite(xin_file, (char*)VGA_TEXT_MEMORY, VGA_SCREEN_RESOLUTION);
    }
    fclose(&xin_file);
    return XANIN_OK;

}

// 21:16:21
// 19:51:50