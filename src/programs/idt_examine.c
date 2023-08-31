
#include <lib/libc/stdlibx.h>
#include <sys/interrupts/idt/idt.h>

extern void divide_by_zero_exception(void);
extern void keyboard_handler_init(void);
extern irq_handler interrupt_handlers[0x100];

int idt_examine(void)
{
    char buf[20] = {0};
    
    puts("\n\n");

    xprintf("  id  %z|%z  handler address\n", OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white));
    putsc("-------------------------------\n", OUTPUT_COLOR_SET(black, green));

    for(int i = 0; i <= UINT8_MAX; i++)
    {
        if(interrupt_handlers[i] != NULL)
        {
            string_align_begin(int_to_str(i, buf), ' ', 4);
            string_align_end(buf, ' ', 6);
            xprintf("%s%z|%z     0x%x\n", buf, OUTPUT_COLOR_SET(black, green), OUTPUT_COLOR_SET(black, white), interrupt_handlers[i]);
        }
    }
    return XANIN_OK;
}