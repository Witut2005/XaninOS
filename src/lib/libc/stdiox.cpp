
#include <lib/libc/memory.h>
#include <lib/libc/pair.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdiox_legacy.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <lib/system/system.h>
#include <stdarg.h>
#include <sys/devices/com/com.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/terminal/backend/backend.h>

extern "C" {

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

    char getchar(void)
    {
        return __sys_getchar();
    }

    char getscan(void)
    {
        return __sys_getscan();
    }

    xchar getxchar(void)
    {
        return __sys_getxchar();
    }

    xchar inputg(void)
    {
        return __sys_inputg();
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

    void xprintf(char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Xtf* vty = __sys_vty_get();

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
                    char format[3];
                    strncpy(format, &fmt[i], 2);
                    char* buf = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

                    xsprintf(buf, format, (char*)va_arg(args, uint32_t));

                    for (int j = 0; buf[j] != '\0'; j++)
                    {
                        __sys_xtf_cell_put(vty, buf[j],
                            OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
                    }

                    free(buf);
                    i = i + 2;
                    break;
                }
                }
            }

            else
            {
                __sys_xtf_cell_put(vty, fmt[i],
                    OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
                i++;
            }
        }
        __sys_xtb_flush(vty);
    }

    struct ScanfArgumentPack
    {
        char* fmt;
        va_list args;
        uint32_t index;
        char* buffer;
        bool blocade;
    };

    void xscanf_handler(KeyInfo key_info, void* args)
    {
        auto vty = __sys_vty_get();
        ScanfArgumentPack* pack = (ScanfArgumentPack*)args;

        if (key_info.scan_code == KBP_BACKSPACE)
        {
            if (pack->index) {
                pack->index--;
            }
            else {
                return;
            }

            __sys_xtf_remove_last_cell(vty);
            __sys_xtb_flush(vty);

            pack->buffer[pack->index] = '\0';
        }

        else if (key_info.scan_code == KBSP_ARROW_UP)
        {
            __sys_xtb_scroll_up(vty);
        }

        else if (key_info.scan_code == KBSP_ARROW_DOWN)
        {
            __sys_xtb_scroll_down(vty);
        }

        else if (key_info.scan_code == KBSP_ARROW_LEFT)
        {
            __sys_xtf_cursor_dec(vty);
        }

        else if (key_info.scan_code == KBSP_ARROW_RIGHT)
        {
            __sys_xtf_cursor_inc(vty);
        }
        else if (key_info.character)
        {
            __sys_xtf_cell_put(vty, key_info.character, OUTPUT_COLOR_SET(black, white));
            pack->buffer[pack->index++] = key_info.character;
        }
        else if (key_info.scan_code == KBP_ENTER)
        {
            strcpy((char*)va_arg(pack->args, uint32_t), pack->buffer);
            __sys_xtf_cell_put(vty, '\n', NULL);
            pack->blocade = false;
        }
    }

    void xscanf(char* fmt, ...)
    {
        if (stdio_mode_get() != STDIO_MODE_TERMINAL)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);

        uint32_t index = 0;

        // char* field_buffer = (char*)calloc(XSCANF_FIELD_BUFFER_SIZE);
        // char* string_typed_buffer = (char*)calloc(XSCANF_STRING_TYPED_BUFFER_SIZE);
        char* buffer = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

        Xtf* vty = __sys_vty_get();
        __sys_xtf_scrolling_on(vty);
        __sys_xtf_cursor_off(vty);

        ScanfArgumentPack pack = { fmt, args, 0, buffer, true };
        InputHandler handler = { xscanf_handler, { &pack, INPUT_USER } };
        auto handler_id = __sys_input_add_handler(&handler);

        while (pack.blocade);
        __sys_input_remove_handler(handler_id);
    }

} // extern "C"
