

#include <fs/xin.h>
#include <lib/libc/canvas.h>
#include <lib/libc/file.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <lib/screen/screen.h>
#include <sys/devices/hda/disk.h>
#include <sys/input/input.h>
#include <sys/terminal/interpreter/interpreter.h>

// CANVAS_APP

void note_input(xchar x)
{

    if (input_is_normal_key_pressed(KBP_F4) | input_is_normal_key_pressed(KBP_ESCAPE))
        app_exited = true;

    else if (x.scan_code == BSPC)
    {

        if (!Screen.x && !Screen.y)
            return;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((black << 4) | white) << 8));

        if (!Screen.x)
        {
            Screen.y--;
            Screen.x = 79;
            return;
        }

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = 0x20;
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) + (((white << 4) | black) << 8));

        return;
    }

    else if (x.scan_code == ARROW_UP)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if (Screen.y > 0)
            Screen.y--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (x.scan_code == ARROW_DOWN)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if (Screen.y < VGA_HEIGHT)
            Screen.y++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (x.scan_code == ARROW_RIGHT)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if (Screen.x == 79 && Screen.y == 27)
            Screen.x--;

        else if (Screen.x == 79)
        {
            Screen.x = 0x0;
            Screen.y++;
        }

        Screen.x++;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (x.scan_code == ARROW_LEFT)
    {

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));

        if (Screen.x == 0x0 && Screen.y == 0x0)
            Screen.x++;

        else if (Screen.x == 0)
        {
            Screen.x = 80;
            Screen.y--;
        }

        Screen.x--;

        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else if (x.scan_code == ENTER)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        canvas_xprintf("\r\n");
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | white) << 8));
    }

    else if (x.scan_code == TAB_KEY)
    {
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((black << 4) | white) << 8));
        Screen.x += 3;
        Screen.cursor[Screen.y][Screen.x] = (uint16_t)((char)(Screen.cursor[Screen.y][Screen.x]) | (((white << 4) | black) << 8));
    }

    else
    {
        if (x.character)
        {
            if (Screen.x == 79 && Screen.y == 27)
                Screen.x--;

            char character_saved_tmp = (char)Screen.cursor[Screen.y][Screen.x];
            canvas_xprintf("%c", getchar());
#warning "letters refresh add function need to be here";
            // letters_refresh_add(&Screen.cursor[Screen.y][Screen.x], character_saved_tmp);
        }
    }
}

int xin_note(char* file_name)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();

    XinEntry* xin_file = fopen(file_name, "rw");

    if (xin_file == NULL)
    {
        canvas_xprintf("Could not open file: %s\n", file_name);
        while (!input_is_normal_key_pressed(KBP_ENTER))
            ;
        return XANIN_ERROR;
    }

    else
    {

        for (int i = 0; i < 16; i++)
            disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, xin_file->first_sector + i, 1, (uint16_t*)((xin_file->first_sector + i) * SECTOR_SIZE));

        canvas_screen_clear();

        char* data_pointer = (char*)(xin_file->first_sector * SECTOR_SIZE);

        uint16_t* bruh_moment = (uint16_t*)VGA_TEXT_MEMORY;

        for (int i = 0; i < VGA_SCREEN_RESOLUTION / 2; i++)
            bruh_moment[i] = (uint16_t)(data_pointer[i] + (((black << 4) | white) << 8));

        while (!app_exited)
            note_input(getxchar());

        uint32_t file_data_counter = 0x0;

        uint16_t* screen_ptr = (uint16_t*)VGA_TEXT_MEMORY;
        uint8_t* tmp = (uint8_t*)malloc(VGA_SCREEN_RESOLUTION);

        for (int i = 0; i < VGA_SCREEN_RESOLUTION / 2; i++, screen_ptr++)
        {
            __xin_fseek(xin_file, i);
            fwrite(xin_file, (char*)screen_ptr, 1);

            tmp[i] = *(char*)screen_ptr;
        }

        xin_file->size = file_data_counter;

        disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, xin_file->first_sector, int_to_sectors(xin_file->size), (uint16_t*)(tmp));

        free(tmp);
    }

    return XANIN_OK;
}
