
/*

#include <stdint.h>
#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <app_config.h>

extern void run16(char* file_name);

#ifdef ZSK_APP
#include <xaninApps/zsk.c>
#endif

#define return_sys(x)\
        asm(\
        "pop ebx\n\t"\
        "pop ecx\n\t"\
        "pop edx\n\t"\
        "pop esi\n\t"\
        "pop edi\n\t");\
        return x


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
   



    eoi_send();
    interrupt_enable();

    switch(eax)
    {

        case 'p' + 's':
        {
            xprintf("%s", (char*)esi);
            return_sys(eax);
        }


        case 'p' + 'd':
        {
            xprintf("%d", esi);
            return_sys(eax); 
        }

        case 'p' + 'x':
        {
            xprintf("0x%x", esi);
            return_sys(eax); 
        }

        case 's' + 's':
        { 
            xscanf("%s",(char*)esi); 
            return_sys(eax);
        }

        case 's' + 'd':
        {
            xscanf("%d", (uint32_t*)esi);
            return_sys(eax);
        }

        case 's' + 'x':
        {
            xscanf("%x", (uint32_t*)esi);
            return_sys(eax);
        }

        case 'z' + 's' + 'k':
        {
            #ifdef ZSK_APP
            zsk(mkstr(1000));
            #endif
            return_sys(eax);
        }


        case 'f' + 'o':
        {
            return_sys(fopen((char*)esi, "rw"));
        }

        case 0xDEAD:
        {
            xprintf("%s", (char*)esi);
            while(1);
            run16((char*)esi);
        }


    }

    

}

*/