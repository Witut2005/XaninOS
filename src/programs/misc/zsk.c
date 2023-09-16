
#include <sys/input/input.h>
#include <lib/libc/canvas.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <lib/libc/string.h>

//CANVAS_APP

void zsk_init(void)
{
    canvas_screen_clear();

    Screen.x = 0;
    Screen.y = 12 ;

    canvas_xprintf("                                                               \n");
    canvas_xprintf("              ############                                     \n");
    canvas_xprintf("          #############                                        \n");
    canvas_xprintf("        #######                                                \n");
    canvas_xprintf("       ##                                                      \n");
    canvas_xprintf("      II    ________                                           \n");
    canvas_xprintf("      II    | ----- |                                          \n");   
    canvas_xprintf("   ___II____| |___| |                                          \n");
    canvas_xprintf("  |                 |     __________      __________           \n");
    canvas_xprintf(" |O|                |    |          |    |          |          \n");
    canvas_xprintf("  |_________________|=||=|    %zZSK%z   |=||=|    %zZSK%z   |=|\n", OUTPUT_COLOR_SET(lblue,white), OUTPUT_COLOR_SET(black, white),OUTPUT_COLOR_SET(lblue,white), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf(" /====|_____|====O |     |__________|    |__________|          \n");
    canvas_xprintf("  {___}      L____/        O      O        O      O            \n");
}

void zsk_move(uint32_t delay)
{
    uint16_t* tmp = (uint16_t*)VGA_TEXT_MEMORY;

    if(*tmp == (uint16_t)('#' + (((black << 4) | white) << 8)))
        zsk_init();

    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
        tmp[i] = tmp[i + 1];

    msleep(delay);

}

int zsk(char* delay_str)
{
    uint32_t delay;
    stdio_mode_set(STDIO_MODE_CANVAS);

    if(!strlen(delay_str))
        delay = 100;
    
    else
        delay = strtoi(delay_str, DECIMAL);

    canvas_xprintf("delay: %d", delay);
    //fwhile(1);

    zsk_init();

    while(KeyInfo.scan_code != ENTER)
        zsk_move(delay);

    app_exited = true;
    return XANIN_OK;
}