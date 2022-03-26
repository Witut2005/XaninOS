

void v86_mode_enter(void)
{
    asm("mov ebp, esp");
    asm("push dword [ebp + 4]");
    asm("push dword [ebp + 8]");
    asm("pushfd");
    asm("or dword [esp], 0x20000");
    asm("push dword [ebp + 12]");
    asm("push dword [ebp + 16]");
    asm("iret");
}