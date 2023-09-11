
#include <sys/terminal/interface/terminal.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <sys/screen/screen.h>

void terminal_set(terminal_t* previous, terminal_t* terminal)
{
    if(previous != (terminal_t*)null_memory_region)
    {
        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
            *(previous->buffer + i) = *(uint8_t*)(VGA_TEXT_MEMORY+i); 

        previous->x = Screen.x;
        previous->y = Screen.y;
    }

    screen_clear();

    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
        *(uint8_t*)(VGA_TEXT_MEMORY+i) = terminal->buffer[i];

    Screen.x = terminal->x;
    Screen.y = terminal->y;

}

terminal_t* terminal_create(void)
{
    terminal_t* new_terminal = (terminal_t*)calloc(SIZE_OF(terminal_t));
    new_terminal->buffer = (uint8_t*)calloc(SIZE_OF(VGA_SCREEN_RESOLUTION));

    new_terminal->x = 0; // i know calloc do this for me but who cares? bla bla bla 
    new_terminal->y = 0;

    return new_terminal;
}

void terminal_destroy(terminal_t* terminal, terminal_t* new_terminal)
{
    free(terminal->buffer);
    free(terminal);
    terminal_set((terminal_t*)null_memory_region, new_terminal);
}


