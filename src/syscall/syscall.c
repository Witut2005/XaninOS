
#include <stdint.h>
#include <lib/stdiox.h>

uint32_t syscall()
{
   
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    

    asm(
        "push eax\n\t"
        "push edi\n\t"
        "push esi\n\t"
        "push edx\n\t"
        "push ecx\n\t"
        "push ebx"
        );

    asm("pop [ecx]" :: "ecx"(&ebx));
    asm("pop [ecx]" :: "ecx"(&ecx));
    asm("pop [ecx]" :: "ecx"(&edx));
    asm("pop [ecx]" :: "ecx"(&esi));
    asm("pop [ecx]" :: "ecx"(&edi));
    asm("pop [ecx]" :: "ecx"(&eax));


    uint32_t eip_ret;
   

    /*
    asm(
            "mov ebx, [esp + 4]\n\t"
            "mov [eax], ebx" :: "eax"(&eip_ret)
        );

    

    asm("mov %0, [esp + 8]\n\t"
        "ret" 
        : "=r"(eip_ret)
        );


    
    xprintf("registers:\n");
    xprintf("eax: 0x%x\n", eax);
    xprintf("edi: 0x%x\n", edi);
    xprintf("esi: 0x%x\n", esi);
    xprintf("edx: 0x%x\n", edx);
    xprintf("eax: 0x%x\n", ecx);
    xprintf("ebx: 0x%x\n", ebx);
    */


    eoi_send();
    asm("sti");

    switch(eax)
    {

        case 0:
        {
            xprintf("%s", (char*)esi);
            break; 
        }


        case 1:
        {
            xprintf("%d", esi);
            break; 
        }

        case 2:
        {
            xprintf("%x", esi);
            break; 
        }

        case 3:
        { 
            xscanf("%s",(char*)esi); 
        }

        case 4:
        {
            return 1234;
        }

    }

    

}
