
#include <lib/libc/string.h>
#include <lib/libc/canvas.h>

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

        for (int i = 0; i < SIZE_OF(tmp); i++)
            tmp[i] = '\0';

        if (str[string_counter] == '%')
        {
            switch (str[++string_counter])
            {
            case 'd':
            {

                number = va_arg(args, int);
                int_to_string(number, temporary_pointer, DECIMAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'y':
            {
                uint32_t time = va_arg(args, int);

                uint32_t time_mask = 0xF0000000;
                uint32_t time_shift = 28;

                for (int i = 0; i < 8; i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2 || i == 4)
                        stdio_legacy_cell_put_with_interpretation('-', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                    stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                }

                break;
            }

            case 't':
            {

                uint16_t time = va_arg(args, int);

                uint16_t time_mask = 0xF000;
                uint16_t time_shift = 12;

                for (int i = 0; i < 4; i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2)
                        stdio_legacy_cell_put_with_interpretation(':', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                    stdio_legacy_cell_put_with_interpretation(((time & time_mask) >> time_shift) + '0', OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
                }
                break;
            }

            case 'b':
            {
                number = va_arg(args, int);
                temporary_pointer = int_to_string(number, tmp, BINARY);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 's':
            {

                stringPtr = va_arg(args, char*);

                if (stringPtr == NULL)
                    break;

                for (int i = 0; stringPtr[i] != '\0'; i++)
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
                font_color = (uint8_t)va_arg(args, int);
                background_color = (font_color & 0xf0) >> 4;
                font_color = font_color & 0x0f;
                break;
            }

            case 'x':
            {
                uint32_t number = va_arg(args, uint32_t);
                int_to_string(number, temporary_pointer, HEXADECIMAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'X':
            {
                uint32_t number = va_arg(args, uint32_t);
                int_to_string(number, temporary_pointer, HEXADECIMAL);

                // toupper(temporary_pointer);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                break;
            }

            case 'o':
            {
                number = va_arg(args, int);
                int_to_string(number, temporary_pointer, OCTAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);
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

            case 'm':
            {

                string_counter++;
                switch (str[string_counter])
                {
                case 'x':
                {

                    uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);
                    xsprintf(temporary_pointer, "%02x", number_hex);

                    for (int i = 0; temporary_pointer[i] != '\0'; i++)
                        stdio_legacy_cell_put_with_interpretation(temporary_pointer[i], OUTPUT_COLOR_SET(background_color, font_color), &Screen.y, &Screen.x);

                    break;
                }

                case 'X':
                {

                    uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);

                    xsprintf(temporary_pointer, "%02X", number_hex);

                    for (int i = 0; temporary_pointer[i] != '\0'; i++)
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

    if (position_change_switch_used) // restore Screen.x and Screen.y
    {
        Screen.y = y;
        Screen.x = x;
    }

    va_end(args);
}

void canvas_xscanf(char* str, ...)
{
    uint32_t str_counter = 0;
    uint32_t counter = 0;

    va_list args;
    va_start(args, str);

    uint32_t index = 0;

    PairUInt8 InitialScreenPosition = { Screen.y, Screen.x };

    char field_buffer[1000];

    char string_typed_buffer[1000];

    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    memset(field_buffer, 0, 1000);
    memset(string_typed_buffer, 0, 1000);

    while (1)
    {

        xchar Input;
        Input = __sys_inputg();

        if (Input.scan_code == BSPC)
        {
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);

            if ((InitialScreenPosition.first == Screen.y) && (InitialScreenPosition.second == Screen.x))
                continue;

            if (!Screen.x)
            {
                Screen.y--;
                Screen.x = 79;
                continue;
            }

            Screen.x--;

            if (index)
                index--;

            string_typed_buffer[index] = '\0';

            stdio_legacy_cell_put('\0', OUTPUT_COLOR_SET(white, white), Screen.y, Screen.x);

            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if (Input.scan_code == ENTER)
        {
            while (str[str_counter] != '\0')
            {
                if (str[str_counter] == '%')
                {
                    str_counter++;
                    switch (str[str_counter])
                    {

                    case 's':
                    {
                        char* string_pointer = va_arg(args, char*);

                        for (char* i = string_pointer; *i != '\0'; i++)
                            *i = '\0';

                        if (!strlen(string_typed_buffer))
                            break;

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        for (int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' '; i++)
                        {
                            if ((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
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

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        *number = strtoi(field_buffer, DECIMAL);

                        if (field_buffer[0] == '-')
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

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        *number = strtoi(field_buffer, HEXADECIMAL);

                        if (field_buffer[0] == '-')
                            *number = *number * -1;

                        break;
                    }

                    case 'b':
                    {

                        uint32_t* number = va_arg(args, uint32_t*);

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
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

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);
            xprintf("\n");
            // free(field_buffer);
            break;
        }

        else if (Input.character)
        {
            char tmp = Input.character;

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);

            xprintf("%c", tmp);

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), Screen.y, Screen.x);
            string_typed_buffer[index] = tmp;
            index++;
        }
    }

    free(field_buffer);
}

void canvas_xscan_range(char* string_buffer, uint32_t how_many_chars)
{

    xchar Input;

    uint32_t counter = 0;
    char* string_pointer;
    char* field_buffer = (char*)calloc(how_many_chars);

    char string_typed_buffer[1000];

    uint32_t index = 0;

    if (stdio_mode_get() != STDIO_MODE_CANVAS)
        return;

    memset(string_typed_buffer, '\0', SIZE_OF(string_typed_buffer));

    while (1)
    {

        Input = __sys_inputg();

        if (Input.scan_code == ENTER)
        {
            string_pointer = string_buffer;

            for (int i = 0; string_pointer[i] != '\0'; i++)
                string_pointer[i] = '\0';

            for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' '; i++)
            {
                field_buffer[i] = string_typed_buffer[counter];
                counter++;
            }

            for (int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' '; i++)
            {
                if ((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
                {
                    string_pointer[i] = '\0';
                    break;
                }

                string_pointer[i] = field_buffer[i];
            }

            memset(field_buffer, 0, 1000);

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);

            xprintf("\n");
            return;
        }

        else if (Input.scan_code == BSPC)
        {
            if (!index)
                continue;

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y, Screen.x);

            if (!Screen.x)
            {
                Screen.y--;
                Screen.x = VGA_WIDTH;
                continue;
            }

            Screen.x--;

            index--;

            string_typed_buffer[index] = '\0';
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), Screen.y, Screen.x);

            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if (Input.character)
        {

            if (index == how_many_chars)
                continue;

            xprintf("%c", Input.character);
            string_typed_buffer[index] = Input.character;
            index++;
        }
    }
    free(field_buffer);
}

void canvas_screen_background_color_set(color_t color)
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