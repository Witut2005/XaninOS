
#include <lib/libc/string.h>
#include <lib/screen/screen.h>
#include <sys/call/xanin_sys/calls/stdio/stdio.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>

void canvas_screen_background_color_set(uint8_t color)
{
    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    uint8_t* vga_ptr = (uint8_t*)VGA_TEXT_MEMORY;

    for (int i = 1; i < VGA_SCREEN_RESOLUTION * 2; i += 2)
    {
        vga_ptr[i] &= 0xF;
        vga_ptr[i] |= color << 4;
    }
}

void canvas_putchar(char character)
{
    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    stdio_canvas_cell_put(character, OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);

    if ((++Screen.x) == VGA_WIDTH)
    {
        Screen.x = 0;
        Screen.y++;
    }
}

void canvas_putchar_color(uint8_t color, char character)
{
    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    stdio_legacy_cell_put(character, color, Screen.y, Screen.x);

    if ((++Screen.x) == VGA_WIDTH)
    {
        Screen.x = 0;
        Screen.y++;
    }
}

void canvas_screen_clear(void)
{
    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    uint16_t* screen_cleaner = (uint16_t*)__vga_buffer_segment_get();
    for (int i = 0; i < (__vga_text_mode_width_get() * __vga_text_mode_height_get()); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }

    stdio_legacy_screen_object_restore_defaults();
}

void canvas_xprintf(char* str, ...)
{
    char tmp[128] = { 0 };
    char* temporary_pointer = tmp;

    uint32_t number;
    char* stringPtr;

    va_list args;
    va_start(args, str);

    uint8_t x = Screen.x;
    uint8_t y = Screen.y;

    bool position_change_switch_used = false;

    uint32_t string_counter = 0;

    uint8_t background_color = black;
    uint8_t font_color = white;

    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    while (str[string_counter])
    {

        for (int i = 0; i < sizeof(tmp); i++)
            tmp[i] = '\0';

        if (str[string_counter] == '%')
        {
            switch (str[++string_counter])
            {
            case 'd': {

                number = va_arg(args, int);
                int_to_string(number, temporary_pointer, STRTOI_DECIMAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'y': {
                uint32_t time = va_arg(args, int);

                uint32_t time_mask = 0xF0000000;
                uint32_t time_shift = 28;

                for (int i = 0; i < 8; i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2 || i == 4)
                        stdio_legacy_cell_put_with_interpretation('-', OUTPUT_COLOR_SET(background_color, font_color),
                            &Screen.y, &Screen.x);

                    stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0',
                        OUTPUT_COLOR_SET(background_color, font_color), &Screen.y,
                        &Screen.x);
                }

                break;
            }

            case 't': {

                uint16_t time = va_arg(args, int);

                uint16_t time_mask = 0xF000;
                uint16_t time_shift = 12;

                for (int i = 0; i < 4; i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2)
                        stdio_legacy_cell_put_with_interpretation(':', OUTPUT_COLOR_SET(background_color, font_color),
                            &Screen.y, &Screen.x);

                    stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0',
                        OUTPUT_COLOR_SET(background_color, font_color), &Screen.y,
                        &Screen.x);
                }
                break;
            }

            case 'b': {
                number = va_arg(args, int);
                temporary_pointer = int_to_string(number, tmp, STRTOI_BINARY);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 's': {

                stringPtr = va_arg(args, char*);

                if (stringPtr == NULL)
                    break;

                for (int i = 0; stringPtr[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        stringPtr[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'i': {
                uint8_t number = (uint8_t)va_arg(args, uint32_t);

                stdio_legacy_cell_put_with_interpretation(
                    ((number & 0xF0) >> 4) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                stdio_legacy_cell_put_with_interpretation(
                    (number & 0x0F) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'c': {
                stdio_legacy_cell_put_with_interpretation(
                    (char)va_arg(args, int), OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                break;
            }

            case 'z': {
                font_color = (uint8_t)va_arg(args, int);
                background_color = (font_color & 0xf0) >> 4;
                font_color = font_color & 0x0f;
                break;
            }

            case 'x': {
                uint32_t number = va_arg(args, uint32_t);
                int_to_string(number, temporary_pointer, STRTOI_HEXADECIMAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'X': {
                uint32_t number = va_arg(args, uint32_t);
                int_to_string(number, temporary_pointer, STRTOI_HEXADECIMAL);

                // toupper(temporary_pointer);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'o': {
                number = va_arg(args, int);
                int_to_string(number, temporary_pointer, STRTOI_OCTAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(
                        temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                break;
            }

            case 'h': // do zrobienia ze pointerami hmmhmm nicho?
            {
                position_change_switch_used = true;
                number = (uint16_t)va_arg(args, uint32_t);

                Screen.y = (number >> 8) & 0xFF;
                Screen.x = number & 0xFF;
                break;
            }

            case 'm': {

                string_counter++;
                switch (str[string_counter])
                {
                case 'x': {

                    uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);
                    xsprintf(temporary_pointer, "%02x", number_hex);

                    for (int i = 0; temporary_pointer[i] != '\0'; i++)
                        stdio_legacy_cell_put_with_interpretation(
                            temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                    break;
                }

                case 'X': {

                    uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);

                    xsprintf(temporary_pointer, "%02X", number_hex);

                    for (int i = 0; temporary_pointer[i] != '\0'; i++)
                        stdio_legacy_cell_put_with_interpretation(
                            temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                    break;
                }
                }
                break;
            }
            }

            string_counter++;
        }

        else
            stdio_legacy_cell_put_with_interpretation(
                str[string_counter++], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
    }

    if (position_change_switch_used) // restore Screen.x and Screen.y
    {
        Screen.y = y;
        Screen.x = x;
    }

    va_end(args);
}
