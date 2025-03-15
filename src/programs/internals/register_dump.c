
#include <lib/libc/hal.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/input/input.h>

// TERMINAL_APP

int reg_dump(void)
{
    reg_t GPRegisters;
    seg_t SRegisters;

    asm("mov ebx, [ebp + 4]\n\t"
        "mov %0, ebx"
        : "=r"(GPRegisters.eip)
        :
        : "eax", "ebx");

    xprintf("\nCPU STATE\n");
    xprintf("ip: 0x%x\n", GPRegisters.eip);

    asm("push eax");
    asm("mov [eax], ebx" ::"a"(&GPRegisters.ebx));
    asm("pop eax");
    asm("mov [ebx], eax" ::"b"(&GPRegisters.eax));
    asm("mov [eax], ecx" ::"a"(&GPRegisters.ecx));
    asm("mov [eax], edx" ::"a"(&GPRegisters.edx));
    asm("mov [eax], esp" ::"a"(&GPRegisters.esp));
    asm("mov [eax], ebp" ::"a"(&GPRegisters.ebp));
    asm("mov [eax], esi" ::"a"(&GPRegisters.esi));
    asm("mov [eax], edi" ::"a"(&GPRegisters.edi));

    asm("mov [eax], es" ::"a"(&SRegisters.es));
    asm("mov [eax], ds" ::"a"(&SRegisters.ds));
    asm("mov [eax], ss" ::"a"(&SRegisters.ss));
    asm("mov [eax], fs" ::"a"(&SRegisters.fs));
    asm("mov [eax], cs" ::"a"(&SRegisters.cs));
    asm("mov [eax], gs" ::"a"(&SRegisters.gs));

    xprintf("\nGENERAL PURPOSE REGISTERS\n");
    xprintf("eax: 0x%x\n", GPRegisters.eax);
    xprintf("ebx: 0x%x\n", GPRegisters.ebx);
    xprintf("ecx: 0x%x\n", GPRegisters.ecx);
    xprintf("edx: 0x%x\n", GPRegisters.edx);
    xprintf("esp: 0x%x\n", GPRegisters.esp);
    xprintf("ebp: 0x%x\n", GPRegisters.ebp);
    xprintf("esi: 0x%x\n", GPRegisters.esi);
    xprintf("edi: 0x%x\n", GPRegisters.edi);

    xprintf("\n");

    xprintf("SEGMENT REGISTERS:\n");
    xprintf("cs: 0x%x\n", SRegisters.cs);
    xprintf("ds: 0x%x\n", SRegisters.ds);
    xprintf("es: 0x%x\n", SRegisters.es);
    xprintf("ss: 0x%x\n", SRegisters.ss);
    xprintf("fs: 0x%x\n", SRegisters.fs);
    xprintf("gs: 0x%x\n", SRegisters.gs);

    return XANIN_OK;
}
