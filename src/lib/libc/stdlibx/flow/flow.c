#include <lib/libc/process.h>
#include <lib/system/system.h>

void exit(void)
{
    // asm("mov eax, 1\n\t"
    //     "int 0x80");
    xanin_syscall1(0x80, 1);
    app_process_unregister();
}
