
#include <stdint.h>
#include <syscall/xanin_sys/xanin_syscalls.h>
#include <libc/stdlibx.h>
#include <libc/memory.h>

#include <syscall/xanin_sys/pmmngr/alloc.h>


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

        // case XANIN_INPUT_GET:
        // {
        //     // ECX = KeyInfo Buf
        //     KeyInfo.scan_code = 0xFF;

        //     while(KeyInfo.scan_code > 0x80); // break codes doesnt count
            
        //     xchar* Info = (xchar*)ecx;

        //     Info->scan_code = KeyInfo.scan_code;
        //     Info->character = KeyInfo.character;

        //     break;
        // }


    }

    return eax;

}