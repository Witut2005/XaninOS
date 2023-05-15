
#include <stdint.h>
#include <sys/call/xanin_sys/xanin_syscalls.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>

#include <sys/call/xanin_sys/pmmngr/alloc.h>
#include <sys/input/input.h>
#include <fs/xin.h>

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
        
        // XinFs

        case XANIN_FOPEN:
        {
            // ECX = file name, EDX = options_str
            eax = (uint32_t)__xin_fopen((char*)ecx, (char*)edx);
            break;
        }

        case XANIN_FREAD:
        {
            eax = __xin_fread((XinEntry*)ecx, (void*)edx, ebx);
            break;
        }

        case XANIN_FWRITE:
        {
            eax = __xin_fwrite((XinEntry*)ecx, (void*)edx, ebx);
            break;
        }

        case XANIN_FCLOSE:
        {
            __xin_fclose((XinEntry**)ecx);
            break;
        }

        case XANIN_OPEN:
        {
            eax = __xin_open((char*)ecx, edx);
            break;
        }

        case XANIN_READ:
        {
            eax = __xin_read(ecx, (void*)edx, ebx);
            break;
        }

        case XANIN_WRITE:
        {
            eax = __xin_write(ecx, (void*)edx, ebx);
            break;
        }

        case XANIN_CLOSE:
        {
            __xin_close(ecx);
            break;
        }



        // Memory Allocation

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
            // xprintf("0x%x\n", ecx);
            mmngr_block_free(USER_HEAP, (void*)ecx);
            break;
        }

        case XANIN_REALLOCATE:
        {    
            // ECX = ptr, EDX = size
            eax = (uint32_t)__sys_realloc((void*)ecx, edx);
            break;
        }

        // Input

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
            *(xchar*)ecx = inputg();
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
            break;
        }


    }

    return eax;

}