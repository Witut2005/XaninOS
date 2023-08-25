
#include <lib/libc/pair.h>
#include <lib/libc/string.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <sys/terminal/vty/vty.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <stdarg.h>
#include <lib/libc/time.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdiox_legacy.h>
#include <sys/terminal/backend/backend.h>

char* color_attributes[16] = {"black", "blue", "green", "cyan", "red", "magenta", "brown", "lgray", "dgray","lblue", "lgreen", "lcyan", "lred", "lmagenta", "yellow", "white"};

void screen_background_color_set(color_t color)
{
    uint8_t* vga_ptr = (uint8_t*)VGA_TEXT_MEMORY;

    for(int i = 1; i < VGA_SCREEN_RESOLUTION * 2; i+=2)
    {
        vga_ptr[i] &= 0xF;
        vga_ptr[i] |= color << 4;
    }
}

int vga_screen_buffer_clear(void)
{
    uint16_t* screen_cleaner = (uint16_t*)__vga_buffer_segment_get();
    for(int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }

    stdio_legacy_screen_object_restore_defaults();
    return XANIN_OK;
}

int screen_clear(void)
{

    if(stdio_mode_get() == STDIO_MODE_CANVAS)
    {
        uint16_t* screen_cleaner = (uint16_t*)__vga_buffer_segment_get();
        for(int i = 0; i < (__vga_text_mode_width_get() * __vga_text_mode_height_get()); i++)
        {
            *screen_cleaner = '\0';
            screen_cleaner++;
        }

        Screen.x = 0;
        Screen.y = 0;
    }

    else if(stdio_mode_get() == STDIO_MODE_TERMINAL)
        xtf_buffer_clear(vty_get());
    return XANIN_OK;
}


void putc(char* str, uint32_t count)
{
    asm("mov eax, 4;"
        "mov ebx, 1;"
        "mov ecx, %0;"
        "mov edx, %1;"
        "int 0x80;"
        :
        : "g"(str), "g"(count)
    );
}

char putchar(char character)
{

    if(stdio_mode_get() == STDIO_MODE_CANVAS)
    {
        stdio_legacy_cell_put(character, OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);

        if((++Screen.x) == VGA_WIDTH)
        {    
            Screen.x = 0;
            Screen.y++;
        }
    }

    else if(stdio_mode_get() == STDIO_MODE_TERMINAL)
    {
        xtb_character_put(vty_get(), character);
        // xtb_flush(vty_get());
    }

    return character;
}

char putchar_color(uint8_t color, char character)
{

    stdio_legacy_cell_put(character, color, &Screen.y, &Screen.x);
        
    Screen.x++;

    if(VGA_WIDTH == 80)
    {    
        Screen.x = 0;
        Screen.y++;
    }
           
    return character;
}

void putst(const char* str)
{
    Xtf* StdioFront = vty_get();

    for(int i = 0; i < strlen(str); i++)
        xtb_character_put(StdioFront, str[i]);
        
    xtb_flush(StdioFront);
}

void putct(const char* str, color_t color)
{
    Xtf* StdioFront = vty_get();

    for(int i = 0; i < strlen(str); i++)
        xtb_cell_put(StdioFront, str[i], color);
        
    xtb_flush(StdioFront);
}

void puts(const char* str)
{

    Xtf* StdioVty = vty_get();

    while(*str != '\0')
        xtb_cell_put(StdioVty, *(str++), OUTPUT_COLOR_SET(black, white));

}

void fprintf(XinEntry* Entry, const char* format, ...)
{
    // if(((int)Entry > 0) && ((int)Entry < 3))

    if(((int)Entry == XANIN_STDOUT) || ((int)Entry == XANIN_STDERR))
    {
        // xprintf(fromat, )
    }
}



void xprintf(char* str, ... )
{

    if(stdio_mode_get() == STDIO_MODE_CANVAS)
    {

        char tmp[128] = {0};
        char* temporary_pointer = tmp;
        
        uint32_t number;
        char* stringPtr;

        va_list args;
        va_start(args,str);

        uint8_t x = Screen.x;
        uint8_t y = Screen.y;

        bool position_change_switch_used = false;

        uint32_t string_counter = 0;

        uint8_t background_color = black;
        uint8_t font_color = white;


        while(str[string_counter])
        {

            for(int i = 0; i < SIZE_OF(tmp); i++)
                tmp[i] = '\0';

            if(str[string_counter] == '%')
            {
                switch(str[++string_counter])
                {
                    case 'd':
                    {

                        number = va_arg(args,int);
                        int_to_str(number,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                        break;
                    }

                    case 'y':
                    {
                        uint32_t time = va_arg(args,int);
                
                        uint32_t time_mask = 0xF0000000;
                        uint32_t time_shift = 28;

                        for(int i = 0; i < 8; i++, time_mask = time_mask >> 4, time_shift -= 4)
                        {
                            if(i == 2 || i == 4)
                                stdio_legacy_cell_put_with_interpretation('-', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                            stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0',  OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        }

                        break;

                    }

                    case 't':
                    {
                    
                        uint16_t time = va_arg(args,int);
                
                        uint16_t time_mask = 0xF000;
                        uint16_t time_shift = 12;

                        for(int i = 0; i < 4; i++, time_mask = time_mask >> 4, time_shift -= 4)
                        {
                            if(i == 2)
                                stdio_legacy_cell_put_with_interpretation(':', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                            stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        }
                        break;
                    }

                    case 'b':
                    {
                        number = va_arg(args,int);
                        temporary_pointer = bin_to_str(number,tmp);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                        break;
                    }

                    case 's':
                    {

                        stringPtr = va_arg(args,char*);

                        if(stringPtr == NULL)
                            break;

                        for(int i = 0; stringPtr[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(stringPtr[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                        break;

                    }


                    case 'i':
                    {
                        uint8_t number = (uint8_t)va_arg(args, uint32_t);

                        stdio_legacy_cell_put_with_interpretation(((number & 0xF0) >> 4) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        stdio_legacy_cell_put_with_interpretation((number & 0x0F) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                    
                        break;
                    }

                    case 'c':
                    {
                        stdio_legacy_cell_put_with_interpretation((char)va_arg(args, int), OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        break;
                    }

                    case 'z':
                    {
                        font_color = (uint8_t)va_arg(args,int);
                        background_color = (font_color & 0xf0) >> 4;
                        font_color = font_color & 0x0f;
                        break;
                    }

                    case 'x':
                    {
                        uint32_t number_hex = va_arg(args,uint32_t);
                        int_to_hex_str(number_hex,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                        break;
                    }
                    
                    case 'X':
                    {
                        number = va_arg(args,int);

                        int_to_hex_str(number,temporary_pointer);
                        toupper(temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                        break;
                    }
                    
                    case 'o':
                    {
                        number = va_arg(args,int);
                        int_to_oct_str(number,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        break;
                    }

                    case 'h': // do zrobienia ze pointerami hmmhmm nicho?
                    {
                        position_change_switch_used = true;
                        number = (uint16_t)va_arg(args,uint32_t);

                        Screen.y = (number >> 8) & 0xFF;
                        Screen.x = number & 0xFF;
                        break;
                    }

                    case 'm':
                    {

                        string_counter++;
                        switch(str[string_counter])
                        {
                            case 'x':
                            {

                                uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                                xint_to_hex_str(number_hex,temporary_pointer, SIZE_OF(uint8_t));

                                for(int i = 0; temporary_pointer[i] != '\0'; i++)
                                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                        
                                break;                            
                            }
                    
                            case 'X':
                            {

                                uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);


                                xint_to_hex_str(number_hex,temporary_pointer, SIZE_OF(uint8_t));
                                toupper(temporary_pointer);
                                
                                for(int i = 0; temporary_pointer[i] != '\0'; i++)
                                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                                break;                            
                            }
                        }
                        break;
                    }
                }

                string_counter++;
            }
            
            else
                stdio_legacy_cell_put_with_interpretation(str[string_counter++], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
        }

        if(position_change_switch_used) // restore Screen.x and Screen.y
        {
            Screen.y = y;
            Screen.x = x;
        }

        va_end(args);
    }

    else if(stdio_mode_get() == STDIO_MODE_TERMINAL)
    {

        char tmp[128];
        memset((uint8_t*)tmp, '\0', SIZE_OF(tmp));
        char* temporary_pointer = tmp;
        
        uint32_t number;

        char* stringPtr;
        Xtf* StdioVty = vty_get();

        if(StdioVty == NULL)
            return;

        va_list args;
        va_start(args,str);

        uint8_t x = StdioVty->x;
        uint8_t y = StdioVty->y;
        uint8_t y_begin = StdioVty->y_begin;

        bool position_change_switch_used = false;

        uint32_t string_counter = 0;

        uint8_t background_color = black;
        uint8_t font_color = white;

        while(str[string_counter])
        {

            for(int i = 0; i < SIZE_OF(tmp); i++)
                tmp[i] = '\0';

            if(str[string_counter] == '%')
            {

                string_counter++;
                switch(str[string_counter])
                {
                    case 'd':
                    {
                        number = va_arg(args,int);

                        int_to_str(number,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }

                    case 'y':
                    {
                        uint32_t time = va_arg(args,int);
                
                        uint32_t time_mask = 0xF0000000;
                        uint32_t time_shift = 28;

                        for(int i = 0; i < 8; i++, time_mask = time_mask >> 4, time_shift -= 4)
                        {
                            if(i == 2 || i == 4)
                                xtb_cell_put(StdioVty, '-', OUTPUT_COLOR_SET(background_color, font_color));

                            // Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time & time_mask) >> time_shift) + '0')  | (((background_color << 4) | font_color) << 8));
                            xtb_cell_put(StdioVty, (((time & time_mask) >> time_shift) + '0'), OUTPUT_COLOR_SET(background_color, font_color));

                        }

                        break;

                    }

                    case 't':
                    {
                    
                        uint16_t time = va_arg(args,int);
                
                        uint16_t time_mask = 0xF000;
                        uint16_t time_shift = 12;

                        for(int i = 0; i < 4; i++, time_mask = time_mask >> 4, time_shift -= 4)
                        {
                            if(i == 2)
                                xtb_cell_put(StdioVty, ':', OUTPUT_COLOR_SET(background_color, font_color));

                            // Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time & time_mask) >> time_shift) + '0')  | (((background_color << 4) | font_color) << 8));
                            xtb_cell_put(StdioVty, (((time & time_mask) >> time_shift) + '0'), OUTPUT_COLOR_SET(background_color, font_color));
                        }
                        break;
                    }

                    case 'b':
                    {
                        number = va_arg(args,int);
                        temporary_pointer = bin_to_str(number,tmp);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }

                    case 's':
                    {
                        stringPtr = va_arg(args, char*);

                        if(stringPtr == NULL)
                            break;

                        for(int i = 0; stringPtr[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, stringPtr[i], OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }


                    case 'i':
                    {
                        uint8_t number = (uint8_t)va_arg(args, uint32_t);
                        xtb_cell_put(StdioVty, ((number & 0xF0) >> 4) + '0', OUTPUT_COLOR_SET(background_color, font_color));
                        xtb_cell_put(StdioVty, (number & 0xF) + '0', OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }

                    case 'c':
                    {
                        xtb_cell_put(StdioVty, (char)va_arg(args, int), OUTPUT_COLOR_SET(background_color, font_color));
                        break;
                    }

                    case 'z':
                    {
                        font_color = (uint8_t)va_arg(args,int);
                        background_color = (font_color & 0xf0) >> 4;
                        font_color = font_color & 0x0f;
                        break;
                    }

                    case 'x':
                    {
                        uint32_t number_hex = va_arg(args,uint32_t);
                        int_to_hex_str(number_hex,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));
                        
                        break;
                    }
                    
                    case 'X':
                    {
                        number = va_arg(args,int);
                        int_to_hex_str(number,temporary_pointer);
                        toupper(temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }
                    
                    case 'o':
                    {
                        number = va_arg(args,int);
                        int_to_oct_str(number,temporary_pointer);

                        for(int i = 0; temporary_pointer[i] != '\0'; i++)
                            xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));

                        break;
                    }

                    case 'h':
                    {
                        // (uint16_t)va_arg(args,uint32_t);
                        break;
                    }

                    case 'm':
                    {

                        string_counter++;
                        switch(str[string_counter])
                        {
                            case 'x':
                            {

                                uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                                xint_to_hex_str(number_hex,temporary_pointer, SIZE_OF(uint8_t));

                                for(int i = 0; temporary_pointer[i] != '\0'; i++)
                                    xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));
                        
                                break;                            
                            }
                    
                            case 'X':
                            {

                                uint8_t number_hex = (uint8_t)va_arg(args,uint32_t);
                                xint_to_hex_str(number_hex,temporary_pointer, SIZE_OF(uint8_t));
                                toupper(temporary_pointer);

                                for(int i = 0; temporary_pointer[i] != '\0'; i++)
                                    xtb_cell_put(StdioVty, temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color));
                                
                                break;                            
                            }

                        }


                        break;
                    }

                }

                string_counter++;
            }

            else
                xtb_cell_put(StdioVty, str[string_counter++], OUTPUT_COLOR_SET(background_color, font_color));



        }

        va_end(args);
        xtb_flush(StdioVty);
    }

}

void xscanf(char* str, ... )
{

    if(stdio_mode_get() == STDIO_MODE_CANVAS)
    {

        uint32_t str_counter = 0;
        uint32_t counter = 0;

        va_list args;
        va_start(args, str);

        uint32_t index = 0;
    
        PairUInt8 InitialScreenPosition = {Screen.y, Screen.x};

        char field_buffer[1000];
        memset(field_buffer, 0, 1000);

        char string_typed_buffer[1000];
        memset(string_typed_buffer, 0, 1000);

        while(1)
        {

            xchar Input;
            __sys_inputg(&Input);

            key_info_t KeyInfo;
            __sys_keyinfo_get(&KeyInfo);

            if(Input.scan_code == BSPC)
            {
                stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);

                if((InitialScreenPosition.first == Screen.y) && (InitialScreenPosition.second == Screen.x))
                    continue;

                if(!Screen.x)
                {
                    Screen.y--;
                    Screen.x = 79;
                    continue;
                }

                Screen.x--;

                if(index)
                    index--;

                string_typed_buffer[index] = '\0';

                stdio_legacy_cell_put('\0', OUTPUT_COLOR_SET(white, white), &Screen.y, &Screen.x);

                KeyInfo.is_bspc = false;
                letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
            }

            else if(Input.scan_code == ENTER)
            {
                while(str[str_counter] != '\0')
                {
                    if(str[str_counter] == '%')
                    {
                        str_counter++;
                        switch(str[str_counter])
                        {

                            case 's':
                            {
                                char* string_pointer = va_arg(args, char*);

                                for(char* i = string_pointer; *i != '\0'; i++)
                                    *i = '\0';

                                if(!strlen(string_typed_buffer))
                                    break;

                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                    
                                for(int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' '; i++)
                                {
                                    if((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
                                    {
                                        string_pointer[i] = '\0'; // invalid ASCII characters
                                        break;
                                    }

                                    string_pointer[i] = field_buffer[i];
                                }

                                counter++;
                                break;
                            }


                            case 'd':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, DECIMAL);                       

                                if(field_buffer[0] == '-')
                                    *number = *number * (-1);

                                break;
                            }

                            case 'c':
                            {

                                char* number = va_arg(args, char*);
                                *number = string_typed_buffer[0]; 
                                break;
                            }

                            case 'x':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, HEXADECIMAL);                       
                                
                                if(field_buffer[0] == '-')
                                    *number = *number * -1;

                                break;
                            }
                        

                            case 'b':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, BINARY);                       

                                break;
                            }


                        }

                    str_counter++;

                    }

                    else
                        str_counter++;
                    

                }

                memset(field_buffer, 0, 1000); 
                memset(string_typed_buffer, 0, 1000);

                stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);
                xprintf("\n");
                // free(field_buffer);
                break;

            }

            else if(Input.character)
            {
                char tmp = Input.character;

                stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);
                char character_saved = (char)(Screen.cursor[Screen.y][Screen.x]);
                
                xprintf("%c", tmp);
        
                stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), &Screen.y, &Screen.x);
                string_typed_buffer[index] = tmp;
                index++;
            }    
        }

        free(field_buffer);
    }

    else if(stdio_mode_get() == STDIO_MODE_TERMINAL)
    {
        uint32_t str_counter = 0;
        uint32_t counter = 0;

        va_list args;
        va_start(args, str);

        uint32_t index = 0;
    
        char* field_buffer = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2 * 5);
        char* string_typed_buffer = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2);

        Xtf* StdioVty = vty_get();
        uint32_t begin_index = StdioVty->size;
        xtf_scrolling_on(StdioVty);
        // xtf_cursor_on(StdioVty, OUTPUT_COLOR_SET(white, black));
        xtf_cursor_off(StdioVty);

        start:

        xtb_flush(StdioVty);

        while(1)
        {

            xchar Input;
            __sys_inputg(&Input);

            key_info_t KeyInfo;
            __sys_keyinfo_get(&KeyInfo);

            if(Input.scan_code == BSPC)
            {
                if(begin_index == StdioVty->size)
                    continue;

                xtf_remove_last_cell(StdioVty);
                // StdioVty->rows_changed[StdioVty->y] = XTF_ROW_CHANGED;
                xtb_flush(StdioVty);

                if(index)
                    index--;

                string_typed_buffer[index] = '\0';

                KeyInfo.is_bspc = false;
            }

            else if(Input.scan_code == ARROW_UP)
            {
                xtb_scroll_up(StdioVty);
            }

            else if(Input.scan_code == ARROW_DOWN)
            {
                xtb_scroll_down(StdioVty);
            }

            else if(Input.scan_code == ARROW_LEFT)
            {
                if(begin_index == StdioVty->Cursor.position)
                    continue;
                xtf_cursor_dec(StdioVty);
            }

            else if(Input.scan_code == ARROW_RIGHT)
            {
                xtf_cursor_inc(StdioVty);
            }

            else if(Input.scan_code == LSHIFT)
                goto start;

            else if(Input.scan_code == ENTER)
            {
                while(str[str_counter] != '\0')
                {
                    
                    memset(field_buffer, 0, SIZE_OF(field_buffer));

                    if(str[str_counter] == '%')
                    {
                        str_counter++;
                        switch(str[str_counter])
                        {

                            case 's':
                            {
                                char* string_pointer = va_arg(args, char*);
                                // xprintf("0x%x\n", string_pointer);

                                for(char* i = string_pointer; *i != '\0'; i++)
                                    *i = '\0';

                                if(!strlen(string_typed_buffer))
                                    break;

                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                    
                                for(int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' '; i++)
                                {
                                    if((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
                                    {
                                        string_pointer[i] = '\0'; // invalid ASCII characters
                                        goto end;
                                    }

                                    string_pointer[i] = field_buffer[i];
                                }

                                end:
                                
                                counter++;
                                break;
                            }


                            case 'd':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, DECIMAL);                       

                                if(field_buffer[0] == '-')
                                    *number = *number * -1;

                                break;
                            }

                            case 'c':
                            {

                                char* number = va_arg(args, char*);
                                *number = string_typed_buffer[0]; 
                                break;
                            }




                            case 'x':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, HEXADECIMAL);                       
                                
                                if(field_buffer[0] == '-')
                                    *number = *number * -1;

                                break;
                            }
                        

                            case 'b':
                            {

                                uint32_t* number = va_arg(args, uint32_t*);
                                
                                for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                                {
                                    field_buffer[i] = string_typed_buffer[counter];
                                    counter++;
                                }
                                
                                *number = strtoi(field_buffer, BINARY);                       

                                break;
                            }


                        }

                    str_counter++;

                    }

                    else
                        str_counter++;
                    

                }

                memset(field_buffer, 0, 1000); 
                memset(string_typed_buffer, 0, 1000);

                break;

            }

            else if(Input.character)
            {
                char tmp = Input.character;

                xprintf("%c", tmp);
                string_typed_buffer[index++] = tmp;
            }    
        }

        xprintf("\n");
        // xtf_cursor_off(StdioVty);
        xtb_flush(StdioVty);
        xtf_cursor_inc(StdioVty);
        
        free(field_buffer);
        free(string_typed_buffer);

    }

}

void xscan_range(char* string_buffer, uint32_t how_many_chars)
{

    if(stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    uint32_t counter = 0;
    char* string_pointer;
    char* field_buffer = (char*)calloc(how_many_chars);

    char string_typed_buffer[1000];
    memset(string_typed_buffer, '\0', SIZE_OF(string_typed_buffer));

    uint32_t index = 0;
    
    PairUInt8 InitialScreenPosition = {Screen.y, Screen.x};
    uint16_t* text_buffer_start = &Screen.cursor[Screen.y][Screen.x];

    while(1)
    {
        xchar Input;
        __sys_inputg(&Input);

        key_info_t KeyInfo;
        __sys_keyinfo_get(&KeyInfo);

        if(Input.scan_code == BSPC)
        {
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);

            if((InitialScreenPosition.first == Screen.y) && (InitialScreenPosition.second == Screen.x))
                continue;

            if(!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
                continue;
            }

            Screen.x--;

            if(index)
                index--;

            string_typed_buffer[index] = '\0';
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), &Screen.y, &Screen.x);

            KeyInfo.is_bspc = false;
            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if(Input.scan_code == ENTER)
        {
            string_pointer = string_buffer;
            
            for(int i = 0; string_pointer[i] != '\0'; i++)
                string_pointer[i] = '\0';

            for(int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
            {
                field_buffer[i] = string_typed_buffer[counter];
                counter++;
            }
                
                
            for(int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' '; i++)
            {
                if((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
                {
                    string_pointer[i] = '\0';
                    break;
                }

                string_pointer[i] = field_buffer[i];
            }

            memset(field_buffer, 0, 1000);

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), &Screen.y, &Screen.x);
            
            xprintf("\n");
            return;
        }

        else if(Input.character)
        {

            if(index == how_many_chars)
                continue;

            xprintf("%c", Input.character);
            string_typed_buffer[index] = Input.character;
            index++;
            
        }    
    }
    free(field_buffer);
}


