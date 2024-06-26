
#include "./hexeditor.h"
#include <lib/libc/canvas.h>
#include <lib/libc/file.h>
#include <lib/libc/string.h>
#include <lib/screen/screen.h>
#include <sys/input/input.h>

// CANVAS_APP

static char* data_pointer;
static uint32_t data_pointer_position;
static uint8_t tmp;

#define HEXEDITOR_BYTES_IN_ROW 16
#define HEXEDITOR_SCREEN_ROW_WIDTH 46

void hexeditor_letters_refresh(void)
{
    for (int i = 0; i < VGA_HEIGHT; i++)
    {
        Screen.x = 51;
        Screen.y = i;

        for (int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            canvas_putchar(data_pointer[(HEXEDITOR_BYTES_IN_ROW * i) + j]);

        canvas_xprintf("\n");
    }
}

void hexeditor_input(xchar UserInput)
{
    if (UserInput.scan_code == ARROW_UP)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if ((uint32_t)&Screen.cursor[Screen.y - 1][Screen.x] < (uint32_t)VGA_TEXT_MEMORY)
        {

            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
            return;
        }

        Screen.y--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

        // data_pointer_position -= 26;

        data_pointer_position -= HEXEDITOR_BYTES_IN_ROW;
    }

    else if (UserInput.scan_code == ARROW_DOWN)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if ((uint32_t)&Screen.cursor[Screen.y + 1][Screen.x] >= (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION))
        {
            Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
            return;
        }

        Screen.y++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));

        data_pointer_position += HEXEDITOR_BYTES_IN_ROW;
    }

    else if (UserInput.scan_code == ARROW_RIGHT)
    {

        if ((uint32_t)&Screen.cursor[Screen.y][Screen.x + 1] > (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION - 8))
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        Screen.x++;

        if (Screen.x >= HEXEDITOR_SCREEN_ROW_WIDTH)
        {
            Screen.x = 0;
            Screen.y++;
            data_pointer_position++;
        }

        else if (Screen.cursor[Screen.y][Screen.x] == (uint16_t)(' ' | (((black << 4) | white) << 8)))
        {
            Screen.x++;
            data_pointer_position++;
        }

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (UserInput.scan_code == ARROW_LEFT)
    {

        if ((uint32_t)&Screen.cursor[Screen.y][Screen.x - 1] < (uint32_t)VGA_TEXT_MEMORY)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if (Screen.x == 0)
        {
            Screen.x = HEXEDITOR_SCREEN_ROW_WIDTH;
            Screen.y--;
            data_pointer_position--;
        }

        else if (Screen.cursor[Screen.y][Screen.x - 1] == (uint16_t)(' ' | (((black << 4) | white) << 8)))
        {
            Screen.x = Screen.x - 2;
            data_pointer_position--;
        }

        else
            Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (UserInput.character != '\0')
    {
        if ((uint32_t)&Screen.cursor[Screen.y][Screen.x] >= (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION - 7))
        {
            Screen.x--;
            return;
        }

        if (Screen.x >= HEXEDITOR_SCREEN_ROW_WIDTH)
        {
            Screen.x = 0;
            Screen.y++;
            data_pointer_position++;
        }

        if (UserInput.character >= 'a' && UserInput.character <= 'f')
        {
            if (Screen.cursor[Screen.y][Screen.x + 1] == (uint16_t)(' ' | (((black << 4) | white) << 8)))
            {
                data_pointer[data_pointer_position] &= 0xF0;
                tmp = UserInput.character - 'a' + 0xa;
                data_pointer[data_pointer_position] += tmp;
            }

            else
            {
                data_pointer[data_pointer_position] &= 0x0F;
                tmp = ((UserInput.character - 'a' + 0xa) << 4);
                data_pointer[data_pointer_position] += tmp;
            }

            canvas_xprintf("%c", UserInput.character);
        }

        else if (UserInput.character >= '0' && UserInput.character <= '9')
        {
            if (Screen.cursor[Screen.y][Screen.x + 1] == (uint16_t)(' ' | (((black << 4) | white) << 8)))
            {

                data_pointer[data_pointer_position] &= 0xF0;
                tmp = UserInput.character - '0';
                data_pointer[data_pointer_position] = data_pointer[data_pointer_position] + tmp;
            }

            else
            {
                data_pointer[data_pointer_position] &= 0x0F;
                tmp = ((UserInput.character - '0') << 4);
                data_pointer[data_pointer_position] = data_pointer[data_pointer_position] + tmp;
            }

            canvas_xprintf("%c", UserInput.character);
        }

        if (Screen.cursor[Screen.y][Screen.x] == (uint16_t)(' ' | (((black << 4) | white) << 8)))
        {
            Screen.x++;
            data_pointer_position++;
        }

        uint8_t x_tmp = Screen.x;
        uint8_t y_tmp = Screen.y;

        hexeditor_letters_refresh();

        Screen.x = x_tmp;
        Screen.y = y_tmp;
    }
}

extern char* argv[5]; // USE HERE FUNC

int hexeditor(char* file_name, char* options)
{

    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();

    data_pointer = (char*)NULL;
    data_pointer_position = 0;
    tmp = 0;

    uint32_t hexeditor_offset = 0;

    if (bstrcmp(options, "-offset"))
        hexeditor_offset = strtoi(argv[3], HEXADECIMAL);

    XinEntry* file = fopen(file_name, "rw");

    if (file == NULL)
    {
        canvas_xprintf("Can't open file %s\n", file_name);
        while (input_is_normal_key_pressed(KBP_ENTER))
            ;

        return XANIN_ERROR;
    }

    data_pointer = (char*)calloc(VGA_SCREEN_RESOLUTION);

    __xin_fseek(file, hexeditor_offset);
    fread(file, data_pointer, VGA_SCREEN_RESOLUTION);

    for (int i = 0; i < VGA_HEIGHT; i++)
    {
        for (int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            canvas_xprintf("%mX ", data_pointer[(HEXEDITOR_BYTES_IN_ROW * i) + j]);

        canvas_xprintf("%z | ", OUTPUT_COLOR_SET(black, green));

        for (int j = 0; j < HEXEDITOR_BYTES_IN_ROW; j++)
            canvas_putchar(data_pointer[(HEXEDITOR_BYTES_IN_ROW * i) + j]);

        canvas_xprintf("\n");
    }

    Screen.x = 0;
    Screen.y = 0;

    while (!input_is_normal_key_pressed(KBP_F4))
        hexeditor_input(getxchar());

    __xin_fseek(file, hexeditor_offset);
    fwrite(file, data_pointer, 512);
    fclose(&file);
    free(data_pointer);
    return XANIN_OK;
}
