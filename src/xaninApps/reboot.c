
void reboot(void)
{
    //asm("jmp 0x0000:0x0000"); /* JUMP TO NULL SEGMENT */
    asm("int 0xFF");
}