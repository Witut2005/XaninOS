

#include <stdint.h>
#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <app_config.h>
#include <syscall/posix/syscall_number.h>

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
    asm("pop [edx]" :: "edx"(&ecx));
    asm("pop [ecx]" :: "ecx"(&edx));
    asm("pop [ecx]" :: "ecx"(&esi));
    asm("pop [ecx]" :: "ecx"(&edi));
    asm("pop [ecx]" :: "ecx"(&eax));


    uint32_t eip_ret;
   
    eoi_send();
    interrupt_enable();

    switch(eax)
    {

        case __NR_read: 
        {
            xscan_range(ecx, edx);
            break;
        }

        case __NR_write:
        {
            for(int i = 0; i < edx; i++)
                xprintf("%c", *(uint8_t*)(ecx + i));

            break;
        }
        


    }

    

}

