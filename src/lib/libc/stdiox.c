
#include <lib/libc/memory.h>
#include <lib/libc/pair.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdiox_legacy.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/system.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <stdarg.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/terminal/backend/backend.h>

// XinEntry* stdin;
// XinEntry* stdout;
// XinEntry* stderr;

void putchar(char c)
{
    __sys_xtf_character_put(__sys_vty_get(), c);
}

void stdio_init(void)
{
    xanin_syscall0(XANIN_SHELL_STDEIO_CREATE);
    // XinEntryCreateArgs args;
    // args.entryname = "stdout";
    // stdout = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDOUT);

    // args.entryname = "/proc/stdin";
    // stdin = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDIN);

    // args.entryname = "/proc/OUTPUT_COLOR_ERROR_SET";
    // stdin = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDIN);
}

void putchar_color(char c, color_t color)
{
    __sys_xtf_cell_put(__sys_vty_get(), c, color);
}

int vga_screen_buffer_clear(void)
{
    uint16_t* screen_cleaner = (uint16_t*)__vga_buffer_segment_get();
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }
    return XANIN_OK;
}

int screen_clear(void)
{
    if (stdio_mode_get() == STDIO_MODE_TERMINAL)
        __sys_xtf_buffer_clear(__sys_vty_get());
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
        : "g"(str), "g"(count));
}

void putsc(char const* str, color_t color)
{
    Xtf* StdioFront = __sys_vty_get();

    for (int i = 0; i < strlen(str); i++)
        __sys_xtf_cell_put(StdioFront, str[i], color);
}

void puts(char const* str)
{
    Xtf* StdioVty = __sys_vty_get();

    while (*str != '\0')
        __sys_xtf_character_put(StdioVty, *(str++));
}

void puts_warning(char const* str)
{
    putsc("[Warning]", OUTPUT_COLOR_SET(black, yellow));
    __sys_xtf_character_put(__sys_vty_get(), ' ');
    puts(str);
}

void puts_error(char const* str)
{
    putsc("[Error]", OUTPUT_COLOR_SET(black, lred));
    __sys_xtf_character_put(__sys_vty_get(), ' ');
    puts(str);
}

#warning "Use here srpintf";
void xprintf(char* str, ...)
{
    char tmp[128];
    memset((uint8_t*)tmp, '\0', sizeof(tmp));
    char* temporary_pointer = tmp;

    char* stringPtr;
    Xtf* StdioVty = __sys_vty_get();

    if (StdioVty == NULL)
        return;

    va_list args;
    va_start(args, str);

    uint32_t string_counter = 0;

    uint8_t background_color = black;
    uint8_t font_color = white;

    while (str[string_counter])
    {

        if (str[string_counter] == '%')
        {

            string_counter++;
            switch (str[string_counter])
            {

            case 'd': {
                uint32_t number = va_arg(args, int);
                int_to_string(number, tmp, DECIMAL);

                for (int i = 0; tmp[i] != '\0'; i++)
                {
                    __sys_xtf_cell_put(StdioVty, tmp[i],
                        OUTPUT_COLOR_SET(background_color, font_color));
                }

                break;
            }

            case 'y': {
                uint32_t time = va_arg(args, int);

                uint32_t time_mask = 0xF0000000;
                uint32_t time_shift = 28;

                for (int i = 0; i < 8;
                     i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2 || i == 4)
                        __sys_xtf_cell_put(StdioVty, '-',
                            OUTPUT_COLOR_SET(background_color, font_color));

                    // Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time &
                    // time_mask) >> time_shift) + '0')  | (((background_color << 4) |
                    // font_color) << 8));
                    __sys_xtf_cell_put(StdioVty,
                        (((time & time_mask) >> time_shift) + '0'),
                        OUTPUT_COLOR_SET(background_color, font_color));
                }

                break;
            }

            case 't': {

                uint16_t time = va_arg(args, int);

                uint16_t time_mask = 0xF000;
                uint16_t time_shift = 12;

                for (int i = 0; i < 4;
                     i++, time_mask = time_mask >> 4, time_shift -= 4)
                {
                    if (i == 2)
                        __sys_xtf_cell_put(StdioVty, ':',
                            OUTPUT_COLOR_SET(background_color, font_color));

                    // Screen.cursor[Screen.y][Screen.x] = (uint16_t)( (((time &
                    // time_mask) >> time_shift) + '0')  | (((background_color << 4) |
                    // font_color) << 8));
                    __sys_xtf_cell_put(StdioVty,
                        (((time & time_mask) >> time_shift) + '0'),
                        OUTPUT_COLOR_SET(background_color, font_color));
                }
                break;
            }

            case 'b': {
                uint32_t number = va_arg(args, int);
                temporary_pointer = int_to_string(number, tmp, BINARY);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                        OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 's': {
                stringPtr = va_arg(args, char*);

                if (stringPtr == NULL)
                    break;

                for (int i = 0; stringPtr[i] != '\0'; i++)
                    __sys_xtf_cell_put(StdioVty, stringPtr[i],
                        OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 'i': {
                uint8_t number = (uint8_t)va_arg(args, uint32_t);
                __sys_xtf_cell_put(StdioVty, ((number & 0xF0) >> 4) + '0',
                    OUTPUT_COLOR_SET(background_color, font_color));
                __sys_xtf_cell_put(StdioVty, (number & 0xF) + '0',
                    OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 'c': {
                __sys_xtf_cell_put(StdioVty, (char)va_arg(args, int),
                    OUTPUT_COLOR_SET(background_color, font_color));
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
                int_to_string(number, temporary_pointer, HEXADECIMAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                        OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 'X': {
                uint32_t number = va_arg(args, uint32_t);
                int_to_string(number, temporary_pointer, HEXADECIMAL);
                toupper(temporary_pointer);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                        OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 'o': {
                uint32_t number = va_arg(args, int);
                int_to_string(number, temporary_pointer, OCTAL);

                for (int i = 0; temporary_pointer[i] != '\0'; i++)
                    __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                        OUTPUT_COLOR_SET(background_color, font_color));

                break;
            }

            case 'h': {
                // (uint16_t)va_arg(args,uint32_t);
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
                        __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                            OUTPUT_COLOR_SET(background_color, font_color));

                    break;
                }

                case 'X': {

                    uint8_t number_hex = (uint8_t)va_arg(args, uint32_t);
                    xsprintf(temporary_pointer, "%02X", number_hex);

                    for (int i = 0; temporary_pointer[i] != '\0'; i++)
                        __sys_xtf_cell_put(StdioVty, temporary_pointer[i],
                            OUTPUT_COLOR_SET(background_color, font_color));

                    break;
                }
                }

                break;
            }
            }

            string_counter++;
        }

        else
            __sys_xtf_cell_put(StdioVty, str[string_counter++],
                OUTPUT_COLOR_SET(background_color, font_color));
    }
    va_end(args);
}

void xscanf(char* str, ...)
{
    if (stdio_mode_get() != STDIO_MODE_TERMINAL)
        return;

    uint32_t str_counter = 0;
    uint32_t counter = 0;

    va_list args;
    va_start(args, str);

    uint32_t index = 0;

#define XSCANF_FIELD_BUFFER_SIZE XANIN_PMMNGR_BLOCK_SIZE * 2 * 5
#define XSCANF_STRING_TYPED_BUFFER_SIZE XANIN_PMMNGR_BLOCK_SIZE * 2 * 5

    char* field_buffer = (char*)calloc(XSCANF_FIELD_BUFFER_SIZE);
    char* string_typed_buffer = (char*)calloc(XSCANF_STRING_TYPED_BUFFER_SIZE);

    Xtf* StdioVty = __sys_vty_get();
    uint32_t begin_index = StdioVty->size;
    __sys_xtf_scrolling_on(StdioVty);
    // __sys_xtf_cursor_on(StdioVty, OUTPUT_COLOR_SET(white, black));
    __sys_xtf_cursor_off(StdioVty);

start:

    __sys_xtb_flush(StdioVty);

    while (1)
    {

        xchar Input;
        Input = __sys_inputg();

        if (Input.scan_code == BSPC)
        {
            if (begin_index == StdioVty->size)
                continue;

            __sys_xtf_remove_last_cell(StdioVty);
            // StdioVty->rows_changed[StdioVty->y] = XTF_ROW_CHANGED;
            __sys_xtb_flush(StdioVty);

            if (index)
                index--;

            string_typed_buffer[index] = '\0';
        }

        else if (Input.scan_code == ARROW_UP)
        {
            __sys_xtb_scroll_up(StdioVty);
        }

        else if (Input.scan_code == ARROW_DOWN)
        {
            __sys_xtb_scroll_down(StdioVty);
        }

        else if (Input.scan_code == ARROW_LEFT)
        {
            if (begin_index == StdioVty->Cursor.position)
                continue;
            __sys_xtf_cursor_dec(StdioVty);
        }

        else if (Input.scan_code == ARROW_RIGHT)
        {
            __sys_xtf_cursor_inc(StdioVty);
        }

        else if (Input.scan_code == LSHIFT)
            goto start;

        else if (Input.scan_code == ENTER)
        {
            while (str[str_counter] != '\0')
            {

                memset(field_buffer, 0, XSCANF_FIELD_BUFFER_SIZE);

                if (str[str_counter] == '%')
                {
                    str_counter++;
                    switch (str[str_counter])
                    {

                    case 's': {
                        char* string_pointer = va_arg(args, char*);
                        // xprintf("0x%x\n", string_pointer);

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

                        for (int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' ';
                             i++)
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

                        *number = strtoi(field_buffer, DECIMAL);

                        if (field_buffer[0] == '-')
                            *number = *number * -1;

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

                        *number = strtoi(field_buffer, HEXADECIMAL);

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

        else if (Input.character)
        {
            char tmp = Input.character;

            xprintf("%c", tmp);
            string_typed_buffer[index++] = tmp;
        }
    }

    puts("\n");
    // __sys_xtf_cursor_off(StdioVty);
    __sys_xtb_flush(StdioVty);
    __sys_xtf_cursor_inc(StdioVty);

    free(field_buffer);
    free(string_typed_buffer);
}

void xscan_range(char* string_buffer, uint32_t how_many_chars)
{

    stdio_mode_t stdio_current_mode = stdio_mode_get();
    xchar Input;

    if (stdio_current_mode != STDIO_MODE_TERMINAL)
        return;

    uint32_t buffer_index = 0;
    char* field_buffer = (char*)calloc(how_many_chars);

    while (1)
    {
        Input = __sys_inputg();

        if (Input.scan_code == ENTER)
        {
            puts("\n");
            return;
        }

        else if (Input.scan_code == BSPC)
        {
            if (!buffer_index)
                continue;

            string_buffer[--buffer_index] = '\0';
            stdio_legacy_cell_put('\0', OUTPUT_COLOR_SET(black, black), Screen.y,
                Screen.x);
            __sys_xtf_remove_last_cell(__sys_vty_get());
            __sys_xtb_flush(__sys_vty_get());
            Screen.x--;
        }

        else if (Input.character)
        {
            if (buffer_index == how_many_chars)
                continue;

            string_buffer[buffer_index] = Input.character;
            __sys_xtf_cell_put(__sys_vty_get(), Input.character,
                OUTPUT_COLOR_SET(black, white));
            __sys_xtb_flush(__sys_vty_get());
            buffer_index++;
            Input.character = 0;
        }
    }
    free(field_buffer);
}

#warning TODO must be finished
void new_xprintf(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    struct {
        uint8_t background;
        uint8_t foreground;
    } font_color = { black, white };

    for (int i = 0; fmt[i] != '\0';)
    {
        if (fmt[i] == '%')
        {
            switch (fmt[i + 1])
            {

            case '\0': {
                break;
            }

            case 'z': {
                uint8_t received_color = (uint8_t)va_arg(args, int);
                font_color.background = (received_color & 0xf0) >> 4;
                font_color.foreground = received_color & 0x0f;
                i = i + 2;
                break;
            }

            default: {
                char* format = strdup(&fmt[i]);
                char* format_end = (char_find(format, CHAR_FIND_LETTERS));
                *(format_end + 1) = '\0';

                char* buf = calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

                sprintf(buf, format, va_arg(args, int));

                for (int j = 0; buf[j] != '\0'; j++)
                {
                    __sys_xtf_cell_put(__sys_vty_get(), buf[j],
                        OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
                }

                free(format);
                free(buf);

                i = ((uint32_t)format_end - (uint32_t)format);

                break;
            }
            }
        }

        else
        {
            __sys_xtf_cell_put(__sys_vty_get(), fmt[i],
                OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
            i++;
        }
    }
    __sys_xtb_flush(__sys_vty_get());
}
