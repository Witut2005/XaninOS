

#include <stdint.h>
#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <app_config.h>
#include <syscall/posix/syscall_number.h>
#include <libc/stdlibx.h>
#include <libc/time.h>
#include <libc/hal.h>

uint32_t syscall_handle(void)
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

    interrupt_enable();
   
    switch(eax)
    {

        case __NR_exit:
        {
            last_command_exit_status = ebx;
            exit();
            break;
        }

        case __NR_read: 
        {
            xscan_range(ecx, edx);
            break;
        }

        case __NR_write:
        {
            int how_many_writed;
            if(!FileDescriptorTable[ebx].is_used)
            {
                for(int i = 0; i < edx; i++)
                    xprintf("%c", *(uint8_t*)(ecx + i));    
            }
            
            else
            {
                how_many_writed = write(ebx, (void*)ecx, edx);
                // xprintf("eax: 0x%x\n", eax);
                // xprintf("ebx: 0x%x\n", ebx);
                // xprintf("ecx: 0x%x\n", ecx);
                // xprintf("edx: 0x%x\n", edx);
                // xprintf("writed: %d\n", how_many_writed);
            }

            break;
        }
        
        case __NR_open:
        {                
            // screen_clear();
            // xprintf("eax: 0x%x\n", eax);
            // xprintf("ebx: 0x%x\n", ebx);
            // xprintf("ecx: 0x%x\n", ecx);
            // xprintf("edx: 0x%x\n", edx);
            eax = open((char*)ebx, ecx); //edx = mode
            break;
        }
        
        case __NR_close:
        {
            close(ebx);
            break;
        }
        
        case __NR_mkdir:   
        {
            __sys_xin_folder_create((char*)ebx);
            break;
        }

        case __NR_rmdir:
        {
            __sys_xin_folder_remove((char*)ebx);
            break;
        }    


        case 100:
        {
            msleep(5000);
            break;
        }


    }

    interrupt_disable();
    return eax;

}

