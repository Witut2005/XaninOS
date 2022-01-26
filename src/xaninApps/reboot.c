

#define NULL_SEGMENT 0x0

void reboot(void)
{
    asm("jmp 0x0000:0x0000"); /* JUMP TO NULL SEGMENT */
}