
#include <lib/libc/canvas.h>
#include <lib/libc/string.h>

static inline void letters_refresh(uint16_t* cursor_current_positon)
{
    cursor_current_positon++;

    for (uint16_t* i = cursor_current_positon; (uint32_t)i < (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION * 2);
         i++)
        *(i - 1) = *i;
}

static inline void letters_refresh_add(uint16_t* cursor_current_positon, char character_saved)
{
    char tmp;

    for (uint16_t* i = cursor_current_positon; (uint32_t)i < (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION * 2);
         i++)
    {
        tmp = *(char*)(i);
        *i = (uint16_t)((char)(character_saved) + (((black << 4) | white) << 8));
        character_saved = tmp;
    }

    *cursor_current_positon = (uint16_t)((char)(*cursor_current_positon) + (((black << 4) | white) << 8));
}

#warning "TODO DOESNT WORK";
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
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y,
                Screen.x);

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

                    case 's': {
                        char* string_pointer = va_arg(args, char*);

                        for (char* i = string_pointer; *i != '\0'; i++)
                            *i = '\0';

                        if (!strlen(string_typed_buffer))
                            break;

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' ';
                             i++)
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

                    case 'd': {

                        uint32_t* number = va_arg(args, uint32_t*);

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' ';
                             i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        *number = strtoi(field_buffer, STRTOI_DECIMAL);

                        if (field_buffer[0] == '-')
                            *number = *number * (-1);

                        break;
                    }

                    case 'c': {

                        char* number = va_arg(args, char*);
                        *number = string_typed_buffer[0];
                        break;
                    }

                    case 'x': {

                        uint32_t* number = va_arg(args, uint32_t*);

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' ';
                             i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        *number = strtoi(field_buffer, STRTOI_HEXADECIMAL);

                        if (field_buffer[0] == '-')
                            *number = *number * -1;

                        break;
                    }

                    case 'b': {

                        uint32_t* number = va_arg(args, uint32_t*);

                        for (int i = 0; string_typed_buffer[counter] != '\0' && string_typed_buffer[counter] != ' ';
                             i++)
                        {
                            field_buffer[i] = string_typed_buffer[counter];
                            counter++;
                        }

                        *number = strtoi(field_buffer, STRTOI_BINARY);

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

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y,
                Screen.x);
            canvas_xprintf("\n");
            // free(field_buffer);
            break;
        }

        else if (Input.character)
        {
            char tmp = Input.character;

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y,
                Screen.x);

            canvas_xprintf("%c", tmp);

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), Screen.y,
                Screen.x);
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

    memset(string_typed_buffer, '\0', sizeof(string_typed_buffer));

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

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y,
                Screen.x);

            canvas_xprintf("\n");
            return;
        }

        else if (Input.scan_code == BSPC)
        {
            if (!index)
                continue;

            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(black, white), Screen.y,
                Screen.x);

            if (!Screen.x)
            {
                Screen.y--;
                Screen.x = VGA_WIDTH;
                continue;
            }

            Screen.x--;

            index--;

            string_typed_buffer[index] = '\0';
            stdio_legacy_cell_put((char)Screen.cursor[Screen.y][Screen.x], OUTPUT_COLOR_SET(white, white), Screen.y,
                Screen.x);

            letters_refresh(&Screen.cursor[Screen.y][Screen.x]);
        }

        else if (Input.character)
        {

            if (index == how_many_chars)
                continue;

            canvas_xprintf("%c", Input.character);
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
