
#include <stdint.h>
#include <sys/call/xanin_sys/xanin_syscalls.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>

#include <sys/call/xanin_sys/pmmngr/alloc.h>
#include <sys/input/input.h>


uint32_t xanin_sys_handle(void)
{
    uint32_t eax, ecx, edx, ebx, esi, edi;

    asm(
        "mov %0, eax;"     
        "mov %1, ecx;"     
        "mov %2, edx;"     
        "mov %3, ebx;"     
  
        : "=g"(eax), "=g"(ecx), "=g"(edx), "=g"(ebx)
        :
        : "ecx", "edx", "ebx"
    );

    // xprintf("eax: %d ", eax);

    switch (eax)
    {
        case XANIN_ALLOCATE:
        {
            // ECX = SIZE
            eax = (uint32_t)__sys_malloc(ecx);
            break;
        }

        case XANIN_CALLOCATE:
        {
            // ECX = SIZE
            eax = (uint32_t)__sys_calloc(ecx);
            break;
        }

        case XANIN_FREE:
        {
            // ECX = PTR
            __sys_free((void*)ecx);
            break;
        }

        case XANIN_REALLOCATE:
        {    
            // ECX = PTR, EDX = SIZE
            eax = (uint32_t)((void*)ecx, edx);
            break;
        }

        case XANIN_GETCHAR: 
        {
            eax = inputc();
            break;
        }

        case XANIN_GETSCAN: 
        {
            eax = inputg().scan_code;
            break;
        }

        case XANIN_INPUTG:
        {
            // ECX = PTR
            // xprintf("ecx 0x%x\n", ecx);
            *(xchar*)ecx = inputg();
            // while(1);
            break;
        }

        // case XANIN_INPUT_GET:
        // {
        //     // ECX = KeyInfo Buf
        //     *(xchar*)ecx = inputg();
        //     xprintf("ecx 0x%x\n", ecx);
        //     break;
        // }

        case XANIN_KEYINFO_GET:
        {
            *(key_info_t*)ecx = KeyInfo;
            // xprintf("ecx 0x%x\n", ecx);
            // while(1);
            break;
        }


    }

    return eax;

}