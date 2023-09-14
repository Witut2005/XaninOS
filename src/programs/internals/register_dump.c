
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/input/input.h>

//TERMINAL_APP

int reg_dump(void)
{
    
    //screen_clear();

    asm("mov ebx, [ebp + 4]\n\t"
        "mov %0, ebx"
        :"=r"(Register.eip)
        :
        :"eax", "ebx"
        );     

    xprintf("\nCPU STATE\n");
    xprintf("ip: 0x%x\n", Register.eip);


    asm("push eax");
    asm("mov [eax], ebx" :: "a"(&Register.ebx));
    asm("pop eax");
    asm("mov [ebx], eax" :: "b"(&Register.eax));
    asm("mov [eax], ecx" :: "a"(&Register.ecx));
    asm("mov [eax], edx" :: "a"(&Register.edx));
    asm("mov [eax], esp" :: "a"(&Register.esp));
    asm("mov [eax], ebp" :: "a"(&Register.ebp));
    asm("mov [eax], esi" :: "a"(&Register.esi));
    asm("mov [eax], edi" :: "a"(&Register.edi));

    asm("mov [eax], es" :: "a"(&SegmentRegister.es));
    asm("mov [eax], ds" :: "a"(&SegmentRegister.ds));
    asm("mov [eax], ss" :: "a"(&SegmentRegister.ss));
    asm("mov [eax], fs" :: "a"(&SegmentRegister.fs));
    asm("mov [eax], cs" :: "a"(&SegmentRegister.cs));
    asm("mov [eax], gs" :: "a"(&SegmentRegister.gs));




    xprintf("\nGENERAL PURPOSE REGISTERS\n");
    xprintf("eax: 0x%x\n", Register.eax);
    xprintf("ebx: 0x%x\n", Register.ebx);    
    xprintf("ecx: 0x%x\n", Register.ecx);
    xprintf("edx: 0x%x\n", Register.edx);
    xprintf("esp: 0x%x\n", Register.esp);
    xprintf("ebp: 0x%x\n", Register.ebp);
    xprintf("esi: 0x%x\n", Register.esi);
    xprintf("edi: 0x%x\n", Register.edi);
       
    xprintf("\n");

    xprintf("SEGMENT REGISTERS:\n");
    xprintf("cs: 0x%x\n", SegmentRegister.cs);
    xprintf("ds: 0x%x\n", SegmentRegister.ds);
    xprintf("es: 0x%x\n", SegmentRegister.es);
    xprintf("ss: 0x%x\n", SegmentRegister.ss);
    xprintf("fs: 0x%x\n", SegmentRegister.fs);
    xprintf("gs: 0x%x\n", SegmentRegister.gs);

    return XANIN_OK;

}
