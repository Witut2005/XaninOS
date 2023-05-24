

#include <stdint.h>
#include <lib/libc/stdiox.h>
#include <fs/xin.h>
#include <app_config.h>
#include <sys/call/posix/syscall_number.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <sys/call/posix/posix.h>
#include <fs/xin_syscalls.h>
#include <sys/terminal/interpreter/interpreter.h>


extern int reboot(void);

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
            // kernel_loop();
            break;
        }

        case __NR_read: 
        {
            if(!FileDescriptorTable[ebx].is_used)
                xscan_range((char*)ecx, edx);

            else
                eax = __xin_read(ebx, (void*)ecx, edx);

            break;
        }

        case __NR_write:
        {
            eax = __xin_write(ebx, (void*)ecx, edx);
            break;
        }
        
        case __NR_lseek:
        {
            lseek(ebx, ecx);
            break;
        }

        case __NR_open:
        {                
            eax = __xin_open((char*)ebx, ecx); //edx = mode
            break;
        }
        
        case __NR_close:
        {
            __xin_close(ebx);
            break;
        }
        
        case __NR_mkdir:   
        {
            char* folder_name = (char*)ebx;
            bool allocated_additional_memory = false;
            
            if(folder_name[strlen(folder_name) - 1] != '/')
            {
                folder_name = (char*)calloc(strlen(folder_name) + 1);
                memcpy(folder_name, (uint8_t*)ebx, strlen((char*)ebx));
                folder_name[strlen(folder_name)] = '/';
                allocated_additional_memory = true;
            }

            __sys_xin_folder_create((char*)folder_name);

            if(allocated_additional_memory)
                free(folder_name);

            break;
        }

        case __NR_break:
        {
            eax = -1;
            break;
        }

        case __NR_rename:
        {
            __sys_xin_entry_move((char*)ebx, (char*)ecx);
            break;
        }

        case __NR_rmdir:
        {
            __sys_xin_folder_remove((char*)ebx);
            break;
        }    

        case __NR_creat:
        {
            eax = __sys_xin_file_create((char*)ebx);
            break;
        }

        case __NR_link:
        {
            eax = __sys_xin_link_create((char*)ebx, (char*)ecx);
            // xprintf("filename: %s\n", (char*)ebx);
            // xprintf("linkname: %s\n", (char*)ecx);
            break;
        }

        
        case __NR_reboot:
        {
            if(ebx == LINUX_REBOOT_MAGIC1 && (ecx == LINUX_REBOOT_MAGIC2 || ecx == LINUX_REBOOT_MAGIC2A || 
                                                ecx == LINUX_REBOOT_MAGIC2B || ecx == LINUX_REBOOT_MAGIC2C))
            {
                reboot();
            }

            break;
        }

        case __NR_unlink:
        {
            eax = __sys_xin_link_remove((char*)ebx);
            break;
        }

        case 0xDE:
        {
            break;
        }

        case 0xDF:
        {
            break;
        }

        case 0xFB:
        {
            break;
        }

        case 0x11D:
        {
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

