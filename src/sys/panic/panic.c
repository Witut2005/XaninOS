
#include <lib/libc/canvas.h>
#include <lib/libc/hal.h>

void kernel_panic(uint32_t error_code)
{
    stdio_mode_set(STDIO_MODE_CANVAS);

    vga_screen_buffer_clear();
    canvas_xprintf("\nOjojoj cos poszlo nie tak. \n");
    canvas_xprintf("Trzymaj tu podpowiadajke\nerror code: 0x%x\n\n\n", error_code);
    canvas_xprintf("%z                _____            %z| Przeznaczeniem naszym                    \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z              /  ___             %z| leciec ciagle wyzej i wyzej.             \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z            /  /  _              %z| Biada ptakom, co spoczna -               \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z          /( /( /(_) ) )         %z| bo noc ich zaskoczy, gdzie na            \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z         (     ___ /  /  )       %z| pustyni i zapomniane prozno z            \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z         (     _____ /    )      %z| nadejsciem dnia drogi szukac             \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z         /(               )      %z| beda. Wiec nie zalujcie tych,            \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z        |               /  |     %z| ktorzy leca, ale raczej litujcie         \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z        |     _______ /    |     %z| sie nad tymi, ktorym sil lub             \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z             /    /     /        %z| ochoty do loty zabraklo. Ruch            \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z           /     %z| |%z    /        %z| to zycie! Trud to zycie! Stokroc         \n", OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, lred), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z                 | |             %z| lepiej cierpiec i zyc niz spac           \n", OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z                 | |             %z|\n", OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z                 | |             %z| Nie zawiode Ciebie Babciu!\n", OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white));
    canvas_xprintf("%z                 | |             %z| Przysiegam :))\n", OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white));
    cpu_halt();
}
