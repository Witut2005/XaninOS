

#include <stdint.h>
#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <app_config.h>
#include <syscall/posix/syscall_number.h>
#include <libc/stdlibx.h>
#include <libc/time.h>
#include <libc/hal.h>

uint32_t syscall(void)
{
   

    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

    asm(
        "mov %0, eax;"     
        "mov %1, ecx;"     
        "mov %2, edx;"     
        "mov %3, ebx;"     
  
        : "=g"(eax), "=g"(edx), "=g"(edx), "=g"(ebx)
        :
        : "ecx", "edx", "ebx"
    );

    uint32_t eip_ret;
   
    eoi_send();
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
            if(!FileDescriptorTable[ebx].is_used)
            {
                for(int i = 0; i < edx; i++)
                    xprintf("%c", *(uint8_t*)(ecx + i));    
            }
            
            else
                write(ebx, (void*)ecx, edx);

            break;
        }
        
        case __NR_open:
        {
            int fd;
            fd = open((char*)ebx, ecx); //edx = mode
            eax = fd;
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

    return eax;

}

